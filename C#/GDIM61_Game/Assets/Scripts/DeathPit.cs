using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;



public class DeathPit : MonoBehaviour
{
    [SerializeField]
    private string m_PlayerTag = "Player";

    [SerializeField]
    private GameObject m_Player;

    [SerializeField]
    private Transform m_SpawnPos;

    private void OnTriggerEnter2D(Collider2D collision)
    {
        if (collision.tag == m_PlayerTag)  //if player enters, this will put the player back to m_SpawnPos
        {
            m_Player.transform.position = m_SpawnPos.transform.position;
        }
    }

    //private void OnTriggerEnter2D(Collider2D collision)
    //{
    //    if (collision.tag == m_PlayerTag)
    //    {
    //        //Destroy(collision.gameObject);

    //        //Maybe call sceneloader to go back to menu or something
    //        SceneManager.LoadScene("MainMenu");
    //    }
    //}
}
