using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EdgeDetector : MonoBehaviour
{
    // Do not forget to set tag for whatever surface enemies are on
    private const string m_GroundTag = "Ground";
    private bool m_isOnGround;

    private void Start()
    {
        m_isOnGround = true;
    }

    public bool EnemyGroundCheck()
    {
        return m_isOnGround;
    }

    //private void OnCollisionEnter2D(Collision2D collision)
    //{
    //    if (collision.gameObject.tag == m_GroundTag)
    //    {
    //        m_isOnGround = true;
    //    }
    //}

    //private void OnCollisionStay2D(Collision2D collision)
    //{
    //    if (collision.gameObject.tag == m_GroundTag)
    //    {
    //        m_isOnGround = true;
    //    }
    //}

    //private void OnCollisionExit2D(Collision2D collision)
    //{
    //    if (collision.gameObject.tag == m_GroundTag)
    //    {
    //        Debug.Log("At the edge");
    //        m_isOnGround = false;
    //    }
    //}

    private void OnTriggerEnter2D(Collider2D other)
    {
        if (other.tag == m_GroundTag)
        {
            m_isOnGround = true;
        }
    }

    private void OnTriggerStay2D(Collider2D other)
    {
        if (other.tag == m_GroundTag)
        {
            m_isOnGround = true;
        }
    }

    private void OnTriggerExit2D(Collider2D collision)
    {
        Debug.Log("At the edge");
        m_isOnGround = false;
    }
}
