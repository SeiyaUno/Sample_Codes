using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class HealthRecover : MonoBehaviour
{
    [SerializeField]
    private int m_HealthNum = 50;

    private void OnTriggerEnter2D(Collider2D collision)
    {
        if (collision.gameObject.tag == "Player")
        {
            PlayerHealth ph = collision.gameObject.GetComponent<PlayerHealth>();
            ph.SetCurrentHealth(ph.GetCurrentHealth() + m_HealthNum);
            Destroy(gameObject);
        }
    }
}
