using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MovingPlatform : MonoBehaviour
//Moving platform will need an edge collider 2d and a rigidbody

{
    [SerializeField] private float movingSpeed;             //Sets how fast the platform will move
    [SerializeField] private int startingPoint;             //The starting position of the platform not really needed
    [SerializeField] private Transform[] desiredPoints;     //A list of gameobjects placed where you want the platform to go towards

    private int i; //Used for the transform list

    // Start is called before the first frame update
    void Start()
    {
        transform.position = desiredPoints[startingPoint].position;
    }


    //Causes the platform to move towards the desired position based on world points
    // Update is called once per frame
    //void Update()
    //{
    //    if (Vector2.Distance(transform.position, desiredPoints[i].position) < 0.02f)
    //    {
    //        i++;
    //        if (i == desiredPoints.Length)
    //        {
    //            i = 0;
    //        }
    //    }

    //    transform.position = Vector2.MoveTowards(transform.position, desiredPoints[i].position, movingSpeed * Time.deltaTime);
    //}

    private void FixedUpdate()
    {
        if (Vector2.Distance(transform.position, desiredPoints[i].position) < 0.02f)
        {
            i++;
            if (i == desiredPoints.Length)
            {
                i = 0;
            }
        }

        transform.position = Vector2.MoveTowards(transform.position, desiredPoints[i].position, movingSpeed * Time.deltaTime);
    }

    //When an object touches the platform it will be come a child of the object to move with it
    private void OnCollisionEnter2D(Collision2D collision)
    {
        collision.transform.SetParent(transform);
    }

    //When the player leaves the platform it will become independent again
    private void OnCollisionExit2D(Collision2D collision)
    {
        collision.transform.SetParent(null);
    }
}
