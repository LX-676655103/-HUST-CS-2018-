using UnityEngine;

public class PacmanMove : MonoBehaviour
{
    //吃豆人的移动速度
    public float speed = 0.35f;
    //吃豆人下一次移动将要去的目的地
    private Vector2 dest = Vector2.zero;

    private void Start()
    {
        //保证吃豆人在游戏刚开始的时候不会动
        dest = transform.position;
    }

    private void FixedUpdate()
    {
        //插值得到要移动到dest位置的下一次移动坐标
        Vector2 temp = Vector2.MoveTowards(transform.position, dest, speed);
        //通过刚体来设置物体的位置
        GetComponent<Rigidbody2D>().MovePosition(temp);
        //必须先达到上一个dest的位置才可以发出新的目的地设置指令
        if ((Vector2)transform.position == dest)
        {
            if ((Input.GetKey(KeyCode.UpArrow) || Input.GetKey(KeyCode.W)) && Valid(Vector2.up))
            {
                dest = (Vector2)transform.position + Vector2.up;
            }
            if ((Input.GetKey(KeyCode.DownArrow) || Input.GetKey(KeyCode.S)) && Valid(Vector2.down))
            {
                dest = (Vector2)transform.position + Vector2.down;
            }
            if ((Input.GetKey(KeyCode.LeftArrow) || Input.GetKey(KeyCode.A)) && Valid(Vector2.left))
            {
                dest = (Vector2)transform.position + Vector2.left;
            }
            if ((Input.GetKey(KeyCode.RightArrow) || Input.GetKey(KeyCode.D)) && Valid(Vector2.right))
            {
                dest = (Vector2)transform.position + Vector2.right;
            }
            //获取移动方向
            Vector2 dir = dest - (Vector2)transform.position;
            //把获取到的移动方向设置给动画状态机
            GetComponent<Animator>().SetFloat("DirX", dir.x);
            GetComponent<Animator>().SetFloat("DirY", dir.y);
        }
    }

    //检测将要去的位置是否可以到达
    private bool Valid(Vector2 dir)
    {
        //记录下当前位置
        Vector2 pos = transform.position;
        //从将要到达的位置向当前位置发射一条射线，并储存下射线信息
        RaycastHit2D hit = Physics2D.Linecast(pos + dir, pos);
        //返回此射线是否打到了吃豆人自身上的碰撞器
        return (hit.collider == GetComponent<Collider2D>());
    }
}
