using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class HideWeapon : MonoBehaviour
{
    private SpriteRenderer m_WeaponSprite;

    //this function should be called before calling Hide() or Back()
    public void GetSpriteRendererFromChild()
    {
        m_WeaponSprite = GetComponentInChildren<SpriteRenderer>();
    }

    //This function hides player's weapon
    public void Hide()
    {
        GetSpriteRendererFromChild();
        if (m_WeaponSprite != null)
        {
            m_WeaponSprite.enabled = false;
        }
        else
        {
            Debug.Log("WeaponSprite Null");
        }
    }

    public void Back()
    {
        GetSpriteRendererFromChild();
        if (m_WeaponSprite != null)
        {
            m_WeaponSprite.enabled = true;
        }
        else
        {
            Debug.Log("WeaponSprite Null");
        }
    }
}
