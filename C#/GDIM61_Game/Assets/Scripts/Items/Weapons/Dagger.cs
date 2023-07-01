using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Dagger : Item
{
    [SerializeField]
    private float m_WeaponPower = 20;

    private PlayerAttack m_Player;

    public void Awake()
    {
        m_Player = GameObject.FindGameObjectWithTag("Player").GetComponent<PlayerAttack>();
    }

    public override void ItemEffect()
    {
        PlayerPrefs.SetString("weapon3", "dagger");
        m_Player.SetToDagger();
        m_Player.SetWeaponPower(m_WeaponPower);
    }
}
