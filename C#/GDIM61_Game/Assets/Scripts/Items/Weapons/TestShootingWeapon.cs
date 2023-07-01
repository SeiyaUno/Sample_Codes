using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TestShootingWeapon : Item
{
    [SerializeField]
    private float m_WeaponPower = 10;

    private PlayerAttack m_Player;

    public void Awake()
    {
        m_Player = GameObject.FindGameObjectWithTag("Player").GetComponent<PlayerAttack>();
    }

    public override void ItemEffect()
    {
        PlayerPrefs.SetString("weapon2", "shooting");
        m_Player.SetToLongDistance();
        m_Player.SetWeaponPower(m_WeaponPower);
    }
}
