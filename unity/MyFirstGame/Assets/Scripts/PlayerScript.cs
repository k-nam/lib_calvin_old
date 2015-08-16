using UnityEngine;
using System.Collections;
using UnityEngine.UI;

public class PlayerScript : MonoBehaviour {
	
	private Rigidbody rb;
	public float speed;

	private int count;
	public Text countText;
	public Text winText;

	// Use this for initialization
	void Start () {
		rb = GetComponent<Rigidbody> ();
		count = 0;
		refreshCountText ();
		winText.text = "";
	}
	
	void refreshCountText() {
		countText.text = "Count: " + count.ToString ();
	}

	void FixedUpdate() {
		float moveHorizontal = Input.GetAxis ("Horizontal");
		float moveVertical = Input.GetAxis ("Vertical");
		
		rb.AddForce (new Vector3(moveHorizontal, 0.0f, moveVertical ) * speed);
	}

	// Destroy everything that enters the trigger
	void OnTriggerEnter(Collider other) {
		if (other.gameObject.CompareTag ("Pickup")) {
			other.gameObject.SetActive(false);
			count = count + 1;
			refreshCountText();
			if (count == 2) {
				winText.text = "YOU WON!";

			}
		}
	}
}
