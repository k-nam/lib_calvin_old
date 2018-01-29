using UnityEngine;

[System.Serializable]
public class Boundary
{
	public float minX, maxX, minY, maxY;
}

public class PlayerController : MonoBehaviour {
	public float speed;
	public float tilt;
	public Boundary boundry;

	public GameObject shot;
	public Transform shotSpawn;
	public float fireRate = 0.5f;
	private float nextFire = 0.0f;

	// Use this for initialization
	void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
		if (Input.GetButton("Fire1") && Time.time > nextFire) {
			nextFire = Time.time + fireRate;
			GameObject clone = Instantiate(shot, shotSpawn.position, Quaternion.Euler(0, 0, 0));
			GetComponent<AudioSource> ().Play ();
		}
	}

	void FixedUpdate()
	{
		float moveHorizontal = Input.GetAxis("Horizontal");
		float moveVertical = Input.GetAxis("Vertical");
		Vector3 movement = new Vector3(moveHorizontal, 0.0f, moveVertical);
		Rigidbody rigidbody = GetComponent<Rigidbody>();
		rigidbody.velocity = movement * speed;
		rigidbody.position = new Vector3(
			Mathf.Clamp(rigidbody.position.x, boundry.minX, boundry.maxX), 
			0,
			Mathf.Clamp(rigidbody.position.z, boundry.minY, boundry.maxY));

		rigidbody.rotation = Quaternion.Euler(rigidbody.velocity.z * tilt, 0.0f, rigidbody.velocity.x * -tilt);
	}
}
