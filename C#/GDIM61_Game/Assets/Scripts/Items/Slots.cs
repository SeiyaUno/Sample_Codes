using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
public class Slots : MonoBehaviour
{
    private Inventory inventory;
    public int InventoryID;

    public KeyCode _KEY;
    public Button _BUTTON;

    private PlayerAttack m_Player;
    // Start is called before the first frame update
    void Start()
    {
        inventory = FindObjectOfType<Inventory>();
        _BUTTON = GetComponent<Button>();

        m_Player = GameObject.FindGameObjectWithTag("Player").GetComponent<PlayerAttack>();

    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetKeyDown(_KEY))
        {
            _BUTTON.onClick.Invoke();
        }
        if(transform.childCount <= 0)
        {
            inventory.isFull[InventoryID] = false;
        }
    }

    //public void RemoveItem()
    //{
    //    foreach (Transform child in transform)
    //    {
    //        GameObject.Destroy(child.gameObject);
    //    }
    //}

    public void SwitchWeapon()
    {
        m_Player.RemoveCurrentWeapon();
        if(inventory.weapons[InventoryID] == "LD")
        {
            Debug.Log("Set to LD");
            m_Player.SetToLongDistance();
        }
        else if (inventory.weapons[InventoryID] == "Melee")
        {
            Debug.Log("Set to melee");
            m_Player.SetToMelee();
        }
        else if (inventory.weapons[InventoryID] == "Dagger")
        {
            Debug.Log("Set to Dagger");
            m_Player.SetToDagger();
        }
    }
}
