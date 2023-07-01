using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class SceneLoader : MonoBehaviour
{
    [SerializeField]
    private string m_SceneToLoad;

    private Inventory inventory;

    public GameObject[] items;

    private string m_PlayerTag = "Player";

    public bool scene_changed;

    private PlayerAttack m_Player;

    private void Start()
    {
        inventory = FindObjectOfType<Inventory>();
        scene_changed = false;

        m_Player = GameObject.FindGameObjectWithTag("Player").GetComponent<PlayerAttack>();

    }

    public virtual void ItemEffect()
    {

    }

    private void Update()
    {
        Scene cur_scene;
        cur_scene = SceneManager.GetActiveScene();
        string cur_scene_name = cur_scene.name;
        Debug.Log(PlayerPrefs.GetString("weapon1"));
        Debug.Log(PlayerPrefs.GetString("weapon2"));
        Debug.Log(PlayerPrefs.GetString("weapon3"));

        if (cur_scene_name == "Level2" || cur_scene_name == "Level3")
        {
            if(scene_changed == false)
            {
                for (int i = 0; i < inventory.slots.Length; i++)
                {
                    if (inventory.isFull[i] == false)
                    {
                        if (PlayerPrefs.GetString("weapon1") == "melee")
                        {
                            
                            Instantiate(items[0], inventory.slots[i].transform, false);
                            inventory.weapons[i] = "Melee";
                            inventory.isFull[i] = true;
                            PlayerPrefs.SetString("weapon1", "weapon1");
                            m_Player.SetToMelee();
                            m_Player.SetWeaponPower(10);


                        }
                        else if (PlayerPrefs.GetString("weapon2") == "shooting")
                        {
                            
                            Instantiate(items[1], inventory.slots[i].transform, false);
                            inventory.weapons[i] = "LD";
                            inventory.isFull[i] = true;
                            PlayerPrefs.SetString("weapon2", "weapon2");
                            m_Player.SetToLongDistance();
                            m_Player.SetWeaponPower(10);

                        }
                        else if (PlayerPrefs.GetString("weapon3") == "dagger")
                        {
                            
                            Instantiate(items[2], inventory.slots[i].transform, false);
                            inventory.weapons[i] = "Dagger";
                            inventory.isFull[i] = true;
                            PlayerPrefs.SetString("weapon3", "weapon3");
                            m_Player.SetToDagger();
                            m_Player.SetWeaponPower(20);

                        }
                    }

                }
                scene_changed = true;
                if (PlayerPrefs.GetString("weapon1") == "weapon1")
                {
                    PlayerPrefs.SetString("weapon1", "melee");
                }
                if (PlayerPrefs.GetString("weapon2") == "weapon2")
                {
                    PlayerPrefs.SetString("weapon2", "shooting");
                }
                if (PlayerPrefs.GetString("weapon3") == "weapon3")
                {
                    PlayerPrefs.SetString("weapon3", "dagger");
                }


            }
            


        }
    }

    private void OnTriggerEnter2D(Collider2D collision)
    {
        if(collision.gameObject.tag == m_PlayerTag)
        {
            PlayerHealth ph = collision.gameObject.GetComponent<PlayerHealth>();
            if(ph != null)
            {
                PlayerPrefs.SetInt("PlayerCurrentHealth", ph.GetCurrentHealth());
                Debug.Log("Health saved " + PlayerPrefs.GetInt("PlayerCurrentHealth"));
            }

            LoadNextScene();
        }
    }

    private void LoadNextScene()
    {
        SceneManager.LoadScene(m_SceneToLoad);
    }
}
