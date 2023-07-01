using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Ladder : MonoBehaviour
{
    [SerializeField]
    private float m_MoveSpeed;

    private bool m_PlayerOnLadder;

    // Start is called before the first frame update
    void Start()
    {
        m_PlayerOnLadder = false;
    }

    // Update is called once per frame
    void Update()
    {
        if(m_PlayerOnLadder)
        {
            if(Input.GetKey(KeyCode.W))
            {
                Vector3 move = new Vector3(0, 1, 0) * m_MoveSpeed * Time.deltaTime;
                transform.Translate(move);
            }
            else if (Input.GetKey(KeyCode.S))
            {
                Vector3 move = new Vector3(0, -1, 0) * m_MoveSpeed * Time.deltaTime;
                transform.Translate(move);
            }
        }
    }

    private void OnTriggerStay2D(Collider2D collision)
    {
        if(collision.gameObject.tag == "Player")
        {
            m_PlayerOnLadder = true;
            //collision.GetComponent<Rigidbody2D>().bodyType = RigidbodyType2D.Static;
            // Make a player child of ladder
            collision.gameObject.transform.parent = gameObject.transform;
        }
    }

    private void OnTriggerExit2D(Collider2D collision)
    {
        if (collision.gameObject.tag == "Player")
        {
            m_PlayerOnLadder = true;
            //collision.GetComponent<Rigidbody2D>().bodyType = RigidbodyType2D.Dynamic;
            // Undo child relationship
            collision.gameObject.transform.parent = null;
        }
    }
}
