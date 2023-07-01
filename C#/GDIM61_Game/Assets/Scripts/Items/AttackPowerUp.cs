using System.Collections;
using System.Collections.Generic;
using UnityEngine;

//The tag of this item should be AttackPowerUp
public class AttackPowerUp : MonoBehaviour
{
    [SerializeField]
    private float m_AdditionalPower = 50;

    private void OnTriggerEnter2D(Collider2D collision)
    {
        if (collision.gameObject.tag == "Player")
        {
            collision.gameObject.GetComponent<PlayerAttack>().AddPlayerPower(m_AdditionalPower);
            Destroy(gameObject);
        }
    }

    //private void OnCollisionEnter2D(Collision2D collision)
    //{
    //    if(collision.gameObject.tag == "Player")
    //    {
    //        collision.gameObject.GetComponent<PlayerAttack>().AddPlayerPower(m_AdditionalPower);
    //    }
    //}
}
