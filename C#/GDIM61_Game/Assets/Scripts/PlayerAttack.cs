using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerAttack : MonoBehaviour
{
    public enum WeaponType {
        melee,
        longDistance,
        dagger,
        none
    }

    [SerializeField]
    private float m_DefaultPlayerPower;
    [SerializeField]
    private float m_DefaultAttackRate;
    [SerializeField]
    private Transform m_AttackPoint;
    [SerializeField]
    private float m_AttackRange;
    [SerializeField]
    private GameObject m_Bullet;
    [SerializeField]
    private Transform m_FirePoint;
    [SerializeField]
    private List<GameObject> m_WeaponList = new List<GameObject>();
    [SerializeField]
    private GameObject m_ThrowDagger;

    private WeaponType m_CurrentWeaponType;
    private HideWeapon m_SetHideObj;
    private float m_CurrentWeaponPower;
    private float m_CurrentPlayerPower;
    private float m_CurrentAttackRate;
    private float nextAttackTime = 0f;

    public LayerMask enemyLayers;

    // Start is called before the first frame update
    void Start()
    {
        m_CurrentPlayerPower = m_DefaultPlayerPower;
        m_CurrentAttackRate = m_DefaultAttackRate;
        m_CurrentWeaponPower = 1.0f;        //This is a temporaly value
        m_CurrentWeaponType = WeaponType.none;
        nextAttackTime = Time.time; 

        m_SetHideObj = GetComponentInChildren<HideWeapon>();
    }

    // Update is called once per frame
    void Update()
    {
        if (Time.time >= nextAttackTime && Input.GetMouseButtonDown(0))  //The input key might be changed in the future
        {
            switch(m_CurrentWeaponType)
            {
                case WeaponType.melee:
                    MeleeAttack();
                    break;
                case WeaponType.longDistance:
                    Shoot();
                    break;
                case WeaponType.dagger:
                    DaggerAttack();
                    break;
            }

            nextAttackTime = Time.time + 1f / m_CurrentAttackRate;
        }
    }

    private void MeleeAttack()
    {
        //Detect enemies in attack range
        //Store all enemies that we hit to the list
        //Debug.Log("Melee called");
        Collider2D[] hitEnemies = Physics2D.OverlapCircleAll(m_AttackPoint.position, m_AttackRange, enemyLayers);

        Debug.Log(hitEnemies.Length);
        //Damage them
        foreach (Collider2D enemy in hitEnemies)
        {
           // Debug.Log("Hit enemy");
            enemy.GetComponent<EnemyHealth>().Takedamage(m_CurrentPlayerPower + m_CurrentWeaponPower);
        }
    }

    private void Shoot()
    {
        Instantiate(m_Bullet, m_FirePoint.position, m_FirePoint.rotation);
    }

    private void DaggerAttack()
    {
        Debug.Log("Dagger attack called");
        Instantiate(m_ThrowDagger, m_FirePoint.position, m_FirePoint.rotation);
    }

    private void SetWeapon(WeaponType wp, GameObject wpObj)
    {
        if (m_CurrentWeaponType != wp)
        {
            RemoveCurrentWeapon();
            m_CurrentWeaponType = wp;
            // need to flip when the direction of player changes
            GameObject obj = Instantiate(wpObj, m_AttackPoint.position, Quaternion.identity);
            obj.transform.parent = m_AttackPoint.transform;

            Debug.Log(m_CurrentWeaponType);
            if (wp == WeaponType.longDistance)
            {
                obj.GetComponent<Collider2D>().enabled = false;
            }
            else if(wp == WeaponType.dagger)
            {
                obj.transform.rotation = Quaternion.Euler(0.0f, 0.0f, 80f);
                obj.GetComponent<Collider2D>().enabled = false;
            }
        }
        else
        {
            m_CurrentWeaponType = wp;
        }

        m_SetHideObj.GetSpriteRendererFromChild();
    }

    public void RemoveCurrentWeapon()
    {
        m_CurrentWeaponType = WeaponType.none;
        foreach (Transform child in m_AttackPoint)
        {
            GameObject.Destroy(child.gameObject);
        }
    }

    public void SetToMelee()
    {
        SetWeapon(WeaponType.melee, m_WeaponList[0]);
        
    }

    public void SetToLongDistance()
    {
        SetWeapon(WeaponType.longDistance, m_WeaponList[1]);
        
    }

    public void SetToDagger()
    {
        SetWeapon(WeaponType.dagger, m_WeaponList[2]);
        
    }

    public void AddPlayerPower(float morePower)
    {
        Debug.Log("Player power increased");
        m_CurrentPlayerPower += morePower;
    }

    public void SetWeaponPower(float p)
    {
        m_CurrentWeaponPower = p;
    }

    void OnDrawGizmosSelected()
    {
        if (m_AttackPoint == null)
        {
            return;
        }

        Gizmos.DrawWireSphere(m_AttackPoint.position, m_AttackRange);
    }
}
