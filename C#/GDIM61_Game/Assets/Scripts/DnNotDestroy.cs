using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DnNotDestroy : MonoBehaviour
{
    private void Awake()
    {
        DontDestroyOnLoad(this);
    }
}
