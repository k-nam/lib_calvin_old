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
		if (rb.velocity.magnitude > maxSpeed) {
			rb.velocity = rb.velocity.normalized * maxSpeed;
		}

	}

	void Update()
	{
		if (Input.GetMouseButtonDown(1))
		{
			RaycastHit hit;
			Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
			if (Physics.Raycast(ray, out hit))
			{
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
}
