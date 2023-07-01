using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Projectile : MonoBehaviour
{
    public float speed = 20.0f;
    public int damage = 10;

    private Rigidbody2D rb;
    private PlayerHealth playerHealth;
    public Sprite destroyed;

    // Start is called before the first frame update
    void Start()
    {
        rb = gameObject.GetComponent<Rigidbody2D>();
    }

    // Update is called once per frame
    void Update()
    {
        rb.velocity = transform.right * speed;
    }

    private IEnumerator DestroyAnim()
    {
        SetSpeed(0);
        GetComponent<SpriteRenderer>().sprite = destroyed;
        yield return new WaitForSeconds(0.1f);
        Destroy(gameObject);
    }

    private void OnTriggerEnter2D(Collider2D collision)
    {
        string collisionTag = collision.gameObject.tag;

        if (collisionTag == "Player" || collisionTag == "Ground")
        {
            if (collisionTag == "Player")
            {
                playerHealth = collision.gameObject.GetComponent<PlayerHealth>();
                playerHealth.TakeDamage(damage);
            }
            
            if (destroyed != null)
            {
                StartCoroutine(DestroyAnim());
            }
            else
            {
                Destroy(gameObject);
            }
        }
    }

    public void SetSpeed(float newSpeed)
    {
        speed = newSpeed;
    }
}
