using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayePrefLoader : MonoBehaviour
{
    [SerializeField]
    private HealthBar m_HealthBar;

    private void OnTriggerEnter2D(Collider2D collision)
    {
        if(collision.gameObject.tag == "Player")
        {
            //Get palyer health from playerpref and reflect it to the health bar
            PlayerHealth ph = collision.gameObject.GetComponent<PlayerHealth>();
            ph.SetCurrentHealth(PlayerPrefs.GetInt("PlayerCurrentHealth"));
            m_HealthBar.SetHealth(ph.GetCurrentHealth());
        }
    }
}
