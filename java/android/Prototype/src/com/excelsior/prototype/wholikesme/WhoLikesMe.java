package com.excelsior.prototype.wholikesme;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Comparator;
import java.util.HashMap;
import java.util.HashSet;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.TreeMap;
import java.util.concurrent.ConcurrentHashMap;

import android.os.AsyncTask;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.ListView;

import com.excelsior.prototype.R;
import com.excelsior.prototype.basicinfo.ListAdapter;
import com.excelsior.prototype.maintab.MainTab;
import com.excelsior.prototype.widget.ListCell;
import com.facebook.FacebookRequestError;
import com.facebook.HttpMethod;
import com.facebook.Request;
import com.facebook.RequestBatch;
import com.facebook.Response;
import com.facebook.Session;
import com.facebook.model.GraphObject;
import com.facebook.model.GraphUser;
import com.facebook.widget.LoginButton;

public class WhoLikesMe extends Fragment implements OnClickListener {
	private static class User {
		User(String id, String name) {
			this.id = id;
			this.name = name;
		}

		final String id;
		final String name;
	}

	private static class Post {
		Post(String postId, String posterId, String message) {
			this.postId = postId;
			this.posterId = posterId;
			this.message = message;
		}

		final String postId;
		final String posterId;
		final String message;
	}

	MainTab mainTabActivity;
	List<ListCell> listCells;
	ListAdapter mAdapter;
	LoginButton mLoginButton;
	ConcurrentHashMap<String, Set<String>> userToFriends = new ConcurrentHashMap<String, Set<String>>();
	ConcurrentHashMap<String, Set<String>> userToPosts = new ConcurrentHashMap<String, Set<String>>();
	ConcurrentHashMap<String, Set<String>> postToLikes = new ConcurrentHashMap<String, Set<String>>();
	ConcurrentHashMap<String, User> userIdToUser = new ConcurrentHashMap<String, User>();
	ConcurrentHashMap<String, Post> postIdToPost = new ConcurrentHashMap<String, Post>();
	String userId;
	boolean isApiCalling;

	public WhoLikesMe() {
		listCells = new ArrayList<ListCell>();
		listCells.add(new ListItem("aaaa", 0));
		listCells.add(new ListItem("bbbb", 0));
	}

	public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
		View view = inflater.inflate(R.layout.who_likes_me, container, false);
		mainTabActivity = (MainTab) getActivity();
		mAdapter = new ListAdapter(mainTabActivity, listCells);
		ListView listView = (ListView) view.findViewById(R.id.who_likes_me__list_view);
		listView.setAdapter(mAdapter);
		setOnClickListenersForButtons(view);
		mLoginButton = (LoginButton) view.findViewById(R.id.who_likes_me__login_button);
		mLoginButton.setReadPermissions(Arrays.asList("user_friends", "read_stream", "user_birthday", "user_activities",
				"read_insights"));
		isApiCalling = false;
		return view;
	}

	private void setOnClickListenersForButtons(View view) {
		Button b = null;
		b = (Button) view.findViewById(R.id.who_likes_me__from_you);
		b.setOnClickListener(this);
		b = (Button) view.findViewById(R.id.who_likes_me__to_you);
		b.setOnClickListener(this);
		b = (Button) view.findViewById(R.id.who_likes_me__crawl);
		b.setOnClickListener(this);
	}

	public void onClick(View v) {
		switch (v.getId()) {
			case R.id.who_likes_me__from_you:
				onClickFromYouButton(v);
				break;
			case R.id.who_likes_me__to_you:
				onClickToYouButton(v);
				break;
			case R.id.who_likes_me__crawl:
				onClickCrawl(v);
				break;
			default:
		}
	}

	public void onClickFromYouButton(View v) {
		Log.d("ui", "onClickFromYou");
		new GetDataTask().execute("FromYou");
	}

	public void onClickToYouButton(View v) {
		new GetDataTask().execute("ToYou");
	}

	private void onClickCrawl(View v) {
		new CrawlTask().execute("");
	}

	public void showToYouList() {
	}

	private void showFromYouList() {
		int totalNumOfLikes = 0;
		for (String postId : postToLikes.keySet()) {
			totalNumOfLikes += postToLikes.get(postId).size();
		}
		Log.d("ui", "FromYou-----userToFriends:" + userToFriends.size() + " userToPosts:" + userToPosts.size()
				+ " postToLikes:" + postToLikes.size() + " userIdToUser:" + userIdToUser.size() + " postIdToPost:"
				+ postIdToPost.size() + " total # of likes " + totalNumOfLikes);
		HashMap<String, Integer> friendIdToNumLikesFromMe = new HashMap<String, Integer>();
		for (String friendId : userToPosts.keySet()) {
			int numLikesFromMe = 0;
			for (String postId : userToPosts.get(friendId)) {
				for (String likerId : postToLikes.get(postId)) {
					if (likerId.equals(userId)) {
						numLikesFromMe++;
					}
				}
			}
			friendIdToNumLikesFromMe.put(friendId, Integer.valueOf(numLikesFromMe));
		}
		List<String> sortedFriendIds = new ArrayList<String>(sortByValues(friendIdToNumLikesFromMe).keySet());
		listCells.clear();
		for (String friendId : sortedFriendIds) {
			listCells.add(new ListItem(userIdToUser.get(friendId).name, friendIdToNumLikesFromMe.get(friendId).intValue()));
		}
		this.mAdapter.notifyDataSetChanged();
		Log.d("ui", "finishedFromYou");
	}

	private void GetUser() {
		if (userId == null) {
			// add the user himself first
			Request.newMeRequest(Session.getActiveSession(), new Request.GraphUserCallback() {
				public void onCompleted(GraphUser user, Response response) {
					FacebookRequestError error = response.getError();
					if (error != null) {
						Log.e("get user", error.getErrorMessage());
						return;
					} else {
						userId = user.getId();
						userIdToUser.put(userId, new User(userId, user.getName()));
						Log.d("get user", "user id is " + userId + " user name is " + user.getName());
					}
				}
			}).executeAndWait();
		} else { // start from a given seed user
			new Request(Session.getActiveSession(), "/" + userId, null, HttpMethod.GET, new Request.Callback() {
				public void onCompleted(Response response) {
					FacebookRequestError error = response.getError();
					if (error != null) {
						Log.e("get user", error.getErrorMessage());
						return;
					} else {
						GraphUser user = response.getGraphObjectAs(GraphUser.class);
						userIdToUser.put(userId, new User(userId, user.getName()));
						Log.d("get user", "user id is " + userId + " user name is " + user.getName());
					}
				}
			}).executeAndWait();
		}
	}

	private void GetRealFriendsOf(String userId, Map<String, Integer> friendToNumLikes) {
		// select people who liked my post
		Map<String, Set<String>> userToPosts = new HashMap<String, Set<String>>();
		Map<String, Post> postIdToPost = new HashMap<String, Post>();
		Map<String, Set<String>> postToLikes = new HashMap<String, Set<String>>();
		GetUserToPosts(Arrays.asList(userId), userToPosts, postIdToPost);
		Set<String> posts = userToPosts.get(userId);
		GetPostToLikes(new ArrayList<String>(posts), postToLikes);
		for (String postId : postToLikes.keySet()) {
			Set<String> likes = postToLikes.get(postId);
			for (String friendId : likes) {
				if (friendToNumLikes.containsKey(friendId) == false) {
					friendToNumLikes.put(friendId, Integer.valueOf(1));
				} else {
					int previousValue = friendToNumLikes.get(friendId).intValue();
					friendToNumLikes.put(friendId, Integer.valueOf(previousValue + 1));
				}
			}
		}
	}

	private void GetUserToFriends(final Map<String, User> userIdToUser, final Map<String, Set<String>> userToFriends) {
		List<Request> requests = new ArrayList<Request>();
		for (final String userId : userIdToUser.keySet()) {
			if (userToFriends.containsKey(userId) == false) {
				userToFriends.put(userId, new HashSet<String>());
			}
			Log.d("get friend", "adding api call for " + userId);
			requests.add(new Request(Session.getActiveSession(), "/" + userId + "/friends", null, HttpMethod.GET,
					new Request.Callback() {
						public void onCompleted(Response response) {
							FacebookRequestError error = response.getError();
							if (error != null) {
								Log.d("get friend", error.getErrorMessage());
								return;
							} else {
								List<GraphObject> apiResult = response.getGraphObject().getPropertyAsList("data", GraphObject.class);
								for (GraphObject friend : apiResult) {
									String friendId = (String) friend.getProperty("id");
									userToFriends.get(userId).add(friendId);
									if (userIdToUser.containsKey(friendId) == false) {
										String friendName = (String) friend.getProperty("name");
										Log.d("get friend", "found friend: " + friendName + " " + friendId);
										userIdToUser.put(friendId, new User(friendId, friendName));
									}
								}
							}
						}
					}));
		}
		Log.d("get friend", "executing batch");
		ExecuteBatch(requests);
	}

	public void GetUserToPosts(final List<String> userIds, final Map<String, Set<String>> userToPosts,
			final Map<String, Post> postIdToPost) {
		List<Request> requests = new ArrayList<Request>();
		Log.d("get post", "userId size is " + userIds.size());
		for (final String userId : userIds) {
			if (userToPosts.containsKey(userId) == false) {
				userToPosts.put(userId, new HashSet<String>());
			}
			requests.add(new Request(Session.getActiveSession(), "/" + userId + "/feed", null, HttpMethod.GET,
					new Request.Callback() {
						public void onCompleted(Response response) {
							FacebookRequestError error = response.getError();
							if (error != null) {
								Log.e("get post", error.getErrorMessage());
								return;
							}
							if (response.getGraphObject() != null) {
								List<GraphObject> apiResult = response.getGraphObject().getPropertyAsList("data", GraphObject.class);
								for (GraphObject object : apiResult) {
									if (object.asMap().containsKey("message")) {
										String postId = (String) object.getProperty("id");
										String message = (String) object.getProperty("message");
										userToPosts.get(userId).add(postId);
										Post post = new Post(postId, userId, message);
										postIdToPost.put(postId, post);
										Log.d("get post", postId + "  " + message);
									} else { // post without message is garbage
										Log.e("get post", "post without message!");
									}
								}
							} else {
								Log.e("get post", "result was null");
							}
						}
					}));
			// 680, 1130 - feed
			// 712, 1321 - post
		}
		Log.d("get post", "executing batch");
		ExecuteBatch(requests);
	}

	private void GetPostToLikes(List<String> postIds, final Map<String, Set<String>> postToLikes) {
		List<Request> requests = new ArrayList<Request>();
		for (final String postId : postIds) {
			if (postToLikes.containsKey(postId) == false) {
				postToLikes.put(postId, new HashSet<String>());
			}
			requests.add(new Request(Session.getActiveSession(), "/" + postId + "/likes", null, HttpMethod.GET,
					new Request.Callback() {
						public void onCompleted(Response response) {
							FacebookRequestError error = response.getError();
							if (error != null) {
								Log.e("get likes", error.getErrorMessage());
								// return;
							}
							if (response.getGraphObject() != null) {
								List<GraphObject> apiResult = response.getGraphObject().getPropertyAsList("data", GraphObject.class);
								for (GraphObject object : apiResult) {
									if (object.asMap().containsKey("id") && object.asMap().containsKey("name")) {
										String likerId = (String) object.getProperty("id");
										String likerName = (String) object.getProperty("name");
										Log.d("get likes", likerName);
										postToLikes.get(postId).add(likerId);
									}
								}
							} else {
								Log.e("get likes", "result was null");
							}
						}
					}));
		}
		ExecuteBatch(requests);
	}

	public void ExecuteBatch(List<Request> requests) {
		if (requests.isEmpty() == true) {
			return;
		}
		RequestBatch batch = new RequestBatch();
		for (int i = 0; i < requests.size(); i++) {
			batch.add(requests.get(i));
			if (i % 50 == 0 || i == requests.size() - 1) {
				Request.executeBatchAndWait(batch);
				batch.clear();
			}
		}
	}

	private class GetDataTask extends AsyncTask<String, Void, Void> {
		String jobName;

		protected Void doInBackground(String... params) {
			jobName = params[0];
			userToFriends.clear();
			userToPosts.clear();
			postToLikes.clear();
			userIdToUser.clear();
			postIdToPost.clear();
			GetUser();
			GetUserToFriends(userIdToUser, userToFriends);
			GetUserToPosts(new ArrayList<String>(userIdToUser.keySet()), userToPosts, postIdToPost);
			GetPostToLikes(new ArrayList<String>(postIdToPost.keySet()), postToLikes);
			return null;
		}

		protected void onPostExecute(Void nothinig) {
			if (jobName.equals("FromYou")) {
				showFromYouList();
			} else if (jobName.equals("ToYou")) {
				showToYouList();
			} else {}
		}
	}

	private class CrawlTask extends AsyncTask<String, Void, Void> {
		protected Void doInBackground(String... params) {
			GetUser();
			List<String> stack = new ArrayList<String>();
			Set<String> set = new HashSet<String>();
			Map<String, Integer> newlyFoundSet = new HashMap<String, Integer>();
			stack.add("6849281");
			set.add("6849281");
			while (stack.isEmpty() == false) {
				newlyFoundSet.clear();
				String thisUser = stack.get(stack.size() - 1);
				GetRealFriendsOf(thisUser, newlyFoundSet);
				stack.remove(stack.size() - 1);
				int totalLikes = 0;
				for (String friend : newlyFoundSet.keySet()) {
					if (set.contains(friend) == false) {
						stack.add(friend);
						set.add(friend);
					}
					totalLikes += newlyFoundSet.get(friend).intValue();
				}
				Log.d("crawling", thisUser + " 's num real friend is " + newlyFoundSet.size() + " num like is " + totalLikes);
			}
			return null;
		}
	}

	// sort in reverse order for convenience
	public static <K extends Comparable<K>, V extends Comparable<V>> Map<K, V> sortByValues(final Map<K, V> map) {
		Comparator<K> valueComparator = new Comparator<K>() {
			public int compare(K k1, K k2) {
				int result = map.get(k1).compareTo(map.get(k2));
				if (result == 0) {
					result = k1.compareTo(k2);
				} else {}
				return -result; // for reverse order
			}
		};
		Map<K, V> sortedByValues = new TreeMap<K, V>(valueComparator);
		sortedByValues.putAll(map);
		return new LinkedHashMap<K, V>(sortedByValues);
	}
}
