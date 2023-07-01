using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class PlayerHealth : MonoBehaviour
{
    [SerializeField]
    private HealthBar healthBar;

    [SerializeField]
    private int m_maxHealth = 100;

    private int m_currentHealth;

    //public Animator animator;
    // Start is called before the first frame update
    void Start()
    {
        m_currentHealth = m_maxHealth;
        healthBar.SetMaxHealth(m_maxHealth);
    }

    //FOR TESTING
    private void Update()
    {
        //if (Input.GetKeyDown(KeyCode.Space))
        //{
        //    TakeDamage(20);
        //}
    }

    public void TakeDamage(int damage)
    {
        m_currentHealth -= damage;
        //animator.SetTrigger("Hurt");

        healthBar.SetHealth(m_currentHealth);

        if (m_currentHealth <= 0)
        {
            Die();
        }
    }

    private void Die()
    {
        Debug.Log("Died");
        //Play Die animation
        //animator.SetTrigger("Death");

        SceneManager.LoadScene("MainMenu");
    }

    public int GetCurrentHealth()
    {
        return m_currentHealth;
    }

    public void SetCurrentHealth(int h)
    {
        if(h > m_maxHealth)
        {
            m_currentHealth = m_maxHealth;
        }
        else
        {
            m_currentHealth = h;
        }
        healthBar.SetHealth(m_currentHealth);
    }
}
