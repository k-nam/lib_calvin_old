using UnityEngine;
using System.Collections;
using UnityEngine.UI;

public class PlayerScript : MonoBehaviour {
	
	private Rigidbody rb;
	private float speed = 30;
	private float maxSpeed = 3;

	private int count;
	public Text countText;
	public Text winText;

	private bool isSelected; 

	// Use this for initialization
	void Start () {
		rb = GetComponent<Rigidbody> ();
		count = 0;
		refreshCountText ();
		winText.text = "";
		isSelected = false;
	}
	
	void refreshCountText() {
		countText.text = "Count: " + count.ToString ();
	}

	void FixedUpdate() {

		float moveHorizontal = Input.GetAxis ("Horizontal");
		float moveVertical = Input.GetAxis ("Vertical");
		
		rb.AddForce (new Vector3(moveHorizontal, 0.0f, moveVertical ) * speed);
		if (rb.velocity.magnitude > maxSpeed) {
			rb.velocity = rb.velocity.normalized * maxSpeed;
		}

	}

	void Update()
	{
		RaycastHit hit;
		Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
		if (Physics.Raycast(ray, out hit))
		{
			if (Input.GetMouseButtonDown (0)) { // select unit
				//Debug.Log("Left click");
				bool isClicked = hit.collider == this.GetComponent<Collider>();
				//Debug.Log("Clicked");
				if (Input.GetKey(KeyCode.LeftShift)) { // addition, subtraction
					if (isClicked) {
						selectSelf(!isSelected);
					}
				} else { // reset selection
					if (isClicked) {
						selectSelf(true);
					} else {
						selectSelf(false);
					}
				}
			} else if (Input.GetMouseButtonDown(1)) { // move unit
				NavMeshAgent agent = GetComponent<NavMeshAgent> ();
				agent.destination = hit.point;		
			}
		}
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
	
	void selectSelf(bool toBeSelected) {
		isSelected = toBeSelected;
		if (toBeSelected) {
			Debug.Log ("Selected");
			GetComponent<Renderer>().material.SetColor("_Color", Color.blue);

		
		} else {
			Debug.Log ("Unselected");
			GetComponent<Renderer>().material.SetColor("_Color", Color.red);
		}
	}
}