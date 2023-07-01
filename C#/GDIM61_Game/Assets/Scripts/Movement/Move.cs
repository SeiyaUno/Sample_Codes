using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;

public class Move : MonoBehaviour
{
    [SerializeField]
    private InputController input = null;

    [SerializeField]
    private float maxSpeed = 4f;

    [SerializeField]
    private float maxAcceleration = 4f;

    [SerializeField]
    private float maxAirAcceleration = 20f;

    //numbers for movement
    private Vector2 direction;
    private Vector2 velocity;
    private Vector2 desiredVelocity;
    private Rigidbody2D rb;
    private Ground ground;

    private float maxSpeedChange;
    private float acceleration;
    private bool onGround;

    private bool isFacingRight;

    //for animation
    private Animator m_Animator;
    private HideWeapon m_HideWeapon;
    private float m_RunAnimMinSpeed = 1f;
    private bool m_WeaponIsHidden;

    // Start is called before the first frame update
    void Awake()
    {
        rb = GetComponent<Rigidbody2D>();
        ground = GetComponent<Ground>();

        m_Animator = GetComponent<Animator>();
        m_HideWeapon = GetComponentInChildren<HideWeapon>();
        m_WeaponIsHidden = false;

        isFacingRight = true;
    }

    // Update is called once per frame
    void Update()
    {
        direction.x = input.RetrieveMoveInput();
        desiredVelocity = new Vector2(direction.x, 0f) * Mathf.Max(maxSpeed - ground.GetFriction(), 0f);

        if (!isFacingRight && direction.x > 0)
        {
            Flip();
        }
        else if (isFacingRight && direction.x < 0)
        {
            Flip();
        }
    }

    private void FixedUpdate()
    {
        onGround = ground.GetOnGround();
        velocity = rb.velocity;

        if(Math.Abs(rb.velocity.x) > m_RunAnimMinSpeed && onGround)
        {
            if(m_WeaponIsHidden == false) { 
                //hide weapons when player is running
                m_HideWeapon.Hide();
                m_WeaponIsHidden = true;
            }
          
            m_Animator.Play("PlayerRunning");
        }
        else if(m_WeaponIsHidden == true)
        {
            m_HideWeapon.Back();
            m_WeaponIsHidden = false;
        }


        acceleration = onGround ? maxAcceleration : maxAirAcceleration;
        maxSpeedChange = acceleration * Time.deltaTime;
        velocity.x = Mathf.MoveTowards(velocity.x, desiredVelocity.x, maxSpeedChange);

        rb.velocity = velocity;
    }

    private void Flip()
    {
        isFacingRight = !isFacingRight;
        gameObject.transform.Rotate(0f, 180f, 0f);
        //transform.Rotate(0f, 180f, 0f);
    }
}
