using System.Collections;
using System.Collections.Generic;
using UnityEngine;

// Make sure you add circle collider to bullet and set as trigger
public class Bullet : MonoBehaviour
{
    [SerializeField]
    private float m_Speed = 20f;
    [SerializeField]
    private float m_BulletDamage = 40;

    private Rigidbody2D m_Rigi;
    // Start is called before the first frame update
    void Start()
    {
        m_Rigi = GetComponent<Rigidbody2D>();
        m_Rigi.velocity = transform.right * m_Speed;
    }

    private void OnTriggerEnter2D(Collider2D collision)
    {
        EnemyHealth eneHealth = collision.GetComponent<EnemyHealth>();

        if(eneHealth != null)
        {
            Debug.Log("Enemy damaged");
            eneHealth.Takedamage(m_BulletDamage);
        }
        Destroy(gameObject);
    }
}
