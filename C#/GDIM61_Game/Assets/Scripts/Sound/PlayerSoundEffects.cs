using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerSoundEffects : MonoBehaviour
{
    [SerializeField]
    private List<AudioClip> m_Clips = new List<AudioClip>();

    private AudioSource m_AudioSource;

    private void Awake()
    {
        m_AudioSource = GetComponent<AudioSource>();
    }

    private void Step()
    {
        AudioClip clip = m_Clips[0];
        m_AudioSource.PlayOneShot(clip);
    }
}
