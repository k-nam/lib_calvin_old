using UnityEngine;
using System.Collections;

public class PickupScript : MonoBehaviour {
	
	// Update is called once per frame
	void Update () {
		transform.Rotate (new Vector3 (0, 1, 0) * Time.deltaTime * 100, Space.World);
	}
}
