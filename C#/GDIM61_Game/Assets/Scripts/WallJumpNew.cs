using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class WallJumpNew : MonoBehaviour
{
    [SerializeField]
    private Transform m_FrontCheck;
    [SerializeField]
    private float m_WallSlidingSpeed;
    [SerializeField]
    private float m_CheckRadius;
    [SerializeField]
    private LayerMask m_TerrainType;
    [SerializeField]
    private float m_XWallForce;
    [SerializeField]
    private float m_YWallForce;
    [SerializeField]
    private float m_WallJumpTime; //How long the forces are applied to the player

    private Ground m_GroundCheck;
    private Rigidbody2D m_Rigi;

    private bool m_IsTouchingFront;
    private bool m_Sliding;
    private bool m_WallJumping;

    // Start is called before the first frame update
    void Start()
    {
        m_GroundCheck = GetComponent<Ground>();
        m_Rigi = GetComponent<Rigidbody2D>();
    }

    // Update is called once per frame
    void Update()
    {
        float input = Input.GetAxisRaw("Horizontal");
        m_IsTouchingFront = Physics2D.OverlapCircle(m_FrontCheck.position, m_CheckRadius, m_TerrainType);
        
        if(m_IsTouchingFront && !m_GroundCheck.GetOnGround() && input != 0)
        {
            //character is touching the wall
            m_Sliding = true;
        }
        else
        {
            m_Sliding = false;
        }

        if (m_Sliding)
        {
            // velocity.y is set between m_WallSlidingSpeed and float.MaxValue
            m_Rigi.velocity = new Vector2(m_Rigi.velocity.x,
                                        Mathf.Clamp(m_Rigi.velocity.y, -m_WallSlidingSpeed, float.MaxValue));

            if (Input.GetKeyDown(KeyCode.UpArrow))
            {
                m_WallJumping = true;
                Invoke("SetWallJumpingToFalse", m_WallJumpTime); //the function is called after the time has passed
            }

            if(m_WallJumping)
            {
                m_Rigi.velocity = new Vector2(m_XWallForce * -input, m_YWallForce);
            }
        }
    }

    private void SetWallJumpingToFalse()
    {
        m_WallJumping = false;
    }

}
