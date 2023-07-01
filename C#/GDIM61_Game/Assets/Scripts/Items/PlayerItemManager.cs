using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;

public class PlayerItemManager : MonoBehaviour{

    //m_AllitemNames and m_ItemObjects must be parallel
    //Add the name of an item to this
    [SerializeField]
    private string[] m_AllItemNames;

    //Add item object to this
    [SerializeField]
    private ItemParent[] m_ItemObjects;

    private List<string> m_ItemsAcquired = new List<string>();

    //Before calling this function, at least one item slot should be available
    public void AddItem(string itemName)
    {
        Debug.Log(itemName + " acquired");
        m_ItemsAcquired.Add(itemName);
    }

    //The name should be the same as the one in the AllItems array
    public void UseItem(string name)
    {
        int index = Array.IndexOf(m_AllItemNames, name);
        m_ItemObjects[index].ItemEffect();
        m_ItemsAcquired.Remove(name);

    }

    public int FindItem(string name)
    {
        return Array.IndexOf(m_AllItemNames, name);
    }

    //This function returns all the available items
    //This will probably be used in items menus in UI
    public List<string> AvailableItems()
    {
        return m_ItemsAcquired;
     }
}
