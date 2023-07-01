using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EnemyAI : MonoBehaviour
{ 
    [SerializeField]
    private Transform           m_target;
    [SerializeField]
    private float                   m_speed;
    [SerializeField]
    private float                   m_detectionDistance;
    [SerializeField]
    private float                   m_attackRange;
    [SerializeField]
    private float                   m_attackRate;
    [SerializeField]
    private Transform           m_attackPoint;
    [SerializeField]
    private float                   m_rangeFromAttackPoint;
    [SerializeField]
    private int                     m_attackDamage;
    [SerializeField]
    private LayerMask        m_playerLayers;
    [SerializeField]
    private float                   m_MinDistance = 1;
    [SerializeField]
    private Transform m_edgeDetector;

    private float                   m_nextAttackTime;
    private EdgeDetector m_GetGroundCheck;
    private bool m_FacingRight;
    private float m_HeadDistance = 2f;

    private void Start()
    {
        m_nextAttackTime = Time.time;
        m_GetGroundCheck = m_edgeDetector.GetComponent<EdgeDetector>();
        m_FacingRight = true;
    }
    // Update is called once per frame
    void Update()
    {
        if (m_target.position.x - transform.position.x < 0 && !(m_target.position.y > transform.position.y + m_HeadDistance))
        {
            //if player is on the left
            if (m_FacingRight)
            {
                transform.Rotate(0f, 180f, 0f);
                m_FacingRight = false;
            }
        }
        else if(!(m_target.position.y > transform.position.y + m_HeadDistance))
        {
            // if player is on the right
            if (!m_FacingRight)
            {
                transform.Rotate(0f, -180f, 0f);
                m_FacingRight = true;
            }
        }

        //start moving if taget is within range and there is a ground on the way
        float btwPE = Vector2.Distance(transform.position, m_target.position);

        //Decide if this should move or not
        if (m_GetGroundCheck.EnemyGroundCheck())
        {
            if (btwPE < m_detectionDistance && btwPE > m_MinDistance)
            {
                transform.Translate(new Vector2(m_speed * Time.deltaTime, 0));
            }
        }

        //start attacking if player is within attack range
        if (Time.time >= m_nextAttackTime && btwPE < m_attackRange)
        {
            Attack();
            m_nextAttackTime = Time.time + 1f / m_attackRate;
        }
    }

    private void Attack()
    {
        //Detect enemies in attack range
        //Store all enemies that we hit to the list
        Collider2D[] hitPlayer = Physics2D.OverlapCircleAll(m_attackPoint.position, m_rangeFromAttackPoint, m_playerLayers);

        //Damage them
        foreach (Collider2D enemy in hitPlayer)
        {
            //Debug.Log("Hit");
            enemy.GetComponent<PlayerHealth>().TakeDamage(m_attackDamage);
        }
    }

    void OnDrawGizmosSelected()
    {
        if (m_attackPoint == null)
        {
            return;
        }

        Gizmos.DrawWireSphere(m_attackPoint.position, m_rangeFromAttackPoint);
    }
}
