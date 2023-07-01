using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ThrowDagger : MonoBehaviour
{
    [SerializeField]
    private float m_Speed = 20f;

    private Rigidbody2D m_Rigi;
    // Start is called before the first frame update
    void Start()
    {
        //transform.rotation = Quaternion.Euler(0.0f, 0.0f, -40f);
        m_Rigi = GetComponent<Rigidbody2D>();
        m_Rigi.velocity = transform.right * m_Speed;
        //m_Rigi.velocity = transform.up * 2;
    }

    private void OnTriggerEnter2D(Collider2D collision)
    {
        EnemyHealth eneHealth = collision.GetComponent<EnemyHealth>();

        if (eneHealth != null)
        {
            Debug.Log("Enemy damaged");
            eneHealth.Takedamage(30);
        }

        if(collision.tag != "Player")
        {
            Destroy(gameObject);
        }
    }
}
