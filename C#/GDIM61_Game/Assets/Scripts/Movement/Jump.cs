using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Jump : MonoBehaviour
{
    [SerializeField]
    private InputController input = null;

    [SerializeField, Range(0f, 10f)]
    private float jumpHeight = 3f;

    [SerializeField, Range(0, 5)]
    private int maxAirJumps = 0;

    [SerializeField, Range(0f, 10f)]
    private float downwardMovementMultiplier = 3f;

    [SerializeField, Range(0f, 10f)]
    private float upwardMovementMultiplier = 1.7f;

    [SerializeField, Range(1f, 3f)]
    private float doubleJumpHeight = 2f;

    [Header("Wall Jump")]

    [SerializeField]
    private Transform frontCheck;

    [SerializeField]
    private float wallSlidingSpeed;

    [SerializeField]
    private LayerMask whatIsGround;

    [SerializeField]
    private float checkRadius;

    private bool wallSliding;
    private bool isTouchingFront = false;

    private Animator m_Animator;

    //Movement variables
    private Rigidbody2D body;
    private Ground ground;
    private Vector2 velocity;

    private int jumpPhase;
    private float defaultGravityScale;

    private bool desiredJump;
    private bool onGround;

    [SerializeField] private float coyoteTime = 0.2f;
    private float coyoteTimeCounter;

    // Start is called before the first frame update
    void Awake()
    {
        body = GetComponent<Rigidbody2D>();
        ground = GetComponent<Ground>();
        m_Animator = GetComponent<Animator>();

        defaultGravityScale = 1f;
    }

    // Update is called once per frame
    void Update()
    {
        desiredJump |= input.RetrieveJumpInput();

        if (Input.GetButtonUp("Jump"))
        {
            SFXManager.PlayClip("jumpSound");
            m_Animator.Play("PlayerJump");
            coyoteTimeCounter = 0f;
        }
    }

    private void FixedUpdate()
    {
        onGround = ground.GetOnGround();
        velocity = body.velocity;

        if (onGround)
        {
            jumpPhase = 0;
            coyoteTimeCounter = coyoteTime;
        }
        else if (coyoteTimeCounter > 0f)
        {
            coyoteTimeCounter -= Time.deltaTime;
        }

        if (desiredJump)
        {
            desiredJump = false;
            JumpAction();
        }

        //Controls gravity moving updward and downward
        if (body.velocity.y > 0)
        {
            body.gravityScale = upwardMovementMultiplier;
        }
        else if (body.velocity.y < 0)
        {
            body.gravityScale = downwardMovementMultiplier;
        }
        else if (body.velocity.y == 0)
        {
            body.gravityScale = defaultGravityScale;
        }

        //Set velocity
        body.velocity = velocity;

    }

    private void JumpAction()
    {
        if (coyoteTimeCounter > 0f || jumpPhase < maxAirJumps)
        {

            float jumpSpeed = onGround
                ? Mathf.Sqrt(-2f * Physics2D.gravity.y * jumpHeight)
                : Mathf.Sqrt(-2f * Physics2D.gravity.y * jumpHeight * doubleJumpHeight);

            if (velocity.y > 0f)
            {
                jumpSpeed = Mathf.Max(jumpSpeed - velocity.y, 0f);
            }

            velocity.y += jumpSpeed;

            if (coyoteTimeCounter > 0f)
            {
                coyoteTimeCounter = 0f;
            }
            else
            {
                jumpPhase += 1;
            }
        }
    }
}
