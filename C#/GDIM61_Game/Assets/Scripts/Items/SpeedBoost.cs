using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SpeedBoost : MonoBehaviour
{
    [SerializeField]
    private float m_AdditionalSpeed;
    [SerializeField]
    private float m_EffectLength;  //The length of time that player can move with faster speed

    private void OnCollisionEnter2D(Collision2D collision)
    {
        if(collision.gameObject.tag == "Player")
        {
            StartCoroutine(IncreaseSpeed());
        }
    }

    private IEnumerator IncreaseSpeed()
    {
        //Need some code
        //Increase speed temporaliry
        yield return new WaitForSeconds(m_EffectLength);
        //back to original speed
    }
}
