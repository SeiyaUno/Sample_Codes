using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;

//Make sure you add the tag "item" to the items

public class Item : MonoBehaviour
{
    //private PlayerItemManager m_Manager;

    private Inventory inventory;

    public GameObject item;

    public bool HasEntered;

    
    // Start is called before the first frame update
    void Start()
    {
        //m_Manager = gameObject.GetComponent<PlayerItemManager>();
        inventory = FindObjectOfType<Inventory>();
        HasEntered = false;

    }

    public virtual void ItemEffect()
    {

    }

    public void OnTriggerEnter2D(Collider2D collision2D)
    {

        ////name should be any name from m_AllItemNames in PlayerItemManager class
        //string name = collision2D.gameObject.tag;

        //int index = m_Manager.FindItem(name);

        //if(index != -1)
        //{
        //    Destroy(collision2D.gameObject);
        //    m_Manager.AddItem(name);
        //}


        if (collision2D.CompareTag("Player") && HasEntered == false)
        {
            ItemEffect();
            if(inventory != null) { Debug.Log("Inventory null"); }
            for (int i = 0; i < inventory.slots.Length; i++)
            {
                if (inventory.isFull[i] == false)
                {
                    Instantiate(item, inventory.slots[i].transform, false);
                    inventory.weapons[i] = gameObject.tag;
                    Destroy(gameObject);
                    inventory.isFull[i] = true;
                    HasEntered = true;
                    break;
                }
            }
        }
    }
}