using UnityEngine;
using System.Collections;

public class UnitTextScript : MonoBehaviour {

	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
		transform.rotation = Quaternion.identity;
		transform.Rotate(new Vector3(90, 0, 0));
	}
}
