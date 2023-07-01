using System.Collections;
using System.Collections.Generic;
using UnityEngine;


//Attack this script to enemy characters
public class EnemyHealth : MonoBehaviour
{
    [SerializeField]
    private float             m_maxHealth = 100f;

    private float             m_currentHealth;

    //public Animator     animator;
    // Start is called before the first frame update
    void Start()
    {
        m_currentHealth = m_maxHealth;
    }

    public void Takedamage(float damage)
    {
        m_currentHealth -= damage;
       // animator.SetTrigger("Hurt");

        if(m_currentHealth <= 0)
        {
            Die();
        }
    }

    private void Die()
    {
        Debug.Log("Enemy Died");
        //Play Die animation
        //animator.SetTrigger("Death");

        //Destroy
        GetComponent<Collider2D>().enabled = false;
        this.enabled = false;
        Destroy(gameObject);
    }
}
