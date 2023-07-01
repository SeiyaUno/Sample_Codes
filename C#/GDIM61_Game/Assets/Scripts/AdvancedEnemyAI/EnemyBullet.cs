using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EnemyBullet : MonoBehaviour
{
    [SerializeField]
    private float m_Speed = 0.2f;

    [SerializeField]
    private int m_Damage = 30;

    private Rigidbody2D m_Rigi;
    // Start is called before the first frame update
    void Start()
    {
        m_Rigi = GetComponent<Rigidbody2D>();
        m_Rigi.velocity = transform.right * m_Speed;
    }

    private void OnTriggerEnter2D(Collider2D collision)
    {
        if(collision.gameObject.tag == "Player")
        {
            Debug.Log("Hit player");
            PlayerHealth playerHealth = collision.GetComponent<PlayerHealth>();

            if (playerHealth != null)
            {
                Debug.Log("Player damaged");
                playerHealth.TakeDamage(m_Damage);
            }
        }
        Destroy(gameObject);
    }
}
