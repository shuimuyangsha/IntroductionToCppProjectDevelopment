#pragma once

#include "Wall.h"
#include "Bullet.h"
#include "GameMap.h"




class CGamePathFinder
{
public:
    CGamePathFinder(const std::vector<std::vector<CGameMapElement>> &arr) : m_arr(arr)
    {
        m_col = m_arr[0].size();
        m_row = m_arr.size();
    }

    ~CGamePathFinder()
    {
    }

    /// ///////////////////////////////////////////////////////////////////////////////
    /// ·�� ����

    // ����֮���Ѱ·:�ж�������ͼ��֮���Ƿ���ͨ
    // ʹ�ù�������㷨��������С·��
    // x,y:��ʼλ��
    // dx,dy:����λ��
    virtual void FindPath(int startX, int startY, int targetX, int targetY, VPath &path) = 0;

protected:

    int m_targetX, m_targetY;
    // ��ͼ������
    // ��ͼ������
    int m_col, m_row;


#define MAX_STEP 99999999
    //��¼��С���� : ��ʼ����һ���ܴ��ֵ�����������ֵ�������ֵ����˵��������ͨ��
    int m_minStep{ MAX_STEP };
    // ��¼ ��Щ·���Ƿ��߹� : �߹�=true,����=false
    std::vector<std::vector<bool>> m_mark;
    // ��¼ ��ʱ����·��
    VPath m_path;
    // ��ͼ������
    const std::vector<std::vector<CGameMapElement>> &m_arr;
};

// ʹ�ù����������
class CPathFinderBFS : public CGamePathFinder
{
public:
    CPathFinderBFS(const std::vector<std::vector<CGameMapElement>> &arr) : CGamePathFinder(arr)
    {

    }

    ~CPathFinderBFS()
    {
    }

    /// ///////////////////////////////////////////////////////////////////////////////
    /// ·�� ����

    // ʹ�ù�������㷨��������С·��
    // x,y:��ʼλ��
    // dx,dy:����λ��
    virtual void FindPath(int startX, int startY, int targetX, int targetY, VPath &path)
    {
        m_targetX = targetX, m_targetY = targetY;
        bfs(startX, startY, targetX, targetY, path);
    }

private:
    //��¼·��
    class CNode
    {
    public:
        CNode()
            : x(0)
            , y(0)
            , s(0)
            , parent(nullptr)
        {
        }

        CNode(int _x, int _y, int _s, CNode *p = nullptr)
            : x(_x)
            , y(_y)
            , s(_s)
            , parent(p)
        {
        }


        CNode(const CNode &rhs)
        {
            if(&rhs != this) {
                this->x = rhs.x;
                this->y = rhs.y;
                this->s = rhs.s;
                this->parent = rhs.parent;
            }
        }
        CNode &operator=(const CNode &rhs)
        {
            if(&rhs != this) {
                this->x = rhs.x;
                this->y = rhs.y;
                this->s = rhs.s;
                this->parent = rhs.parent;
            }
            return *this;
        }
        //~CNode() {
        //    if (parent) {
        //        delete parent;
        //        parent = nullptr;
        //    }
        //}

        int x, y; // ����
        int s; // ����
        shared_ptr<CNode> parent; // �����
    };

    // �������������������
    //  x,y ��ǰ���ڵ��У���
    bool bfs_item(int x, int y)
    {
        bool bFind = false;
        // �潲δ���ʵĿ���ͨ�Ľ��
        std::queue<CNode> q_open;
        // �����뵽������
        q_open.push(CNode(x, y, 0, nullptr));
        // ����߹���
        m_mark[x][y] = true;
        // �����в�Ϊ�յ�ʱ��
        while(!q_open.empty()) {
            // ͷ���ĳ��ӣ��������
            auto curr = q_open.front();
            q_open.pop();
            // ����Ŀ�ĵ�
            if(curr.x == m_targetX && curr.y == m_targetY) {
                m_minStep = curr.s;
                CNode *p = &curr; //��ǰ�����Ŀ�ĵ�
                while(p) {
                    PathNode n;
                    n.x = p->x;
                    n.y = p->y;
                    m_path.push_back(n);
                    p = p->parent.get();
                }
                bFind = true;
                while (!q_open.empty()) {
                    q_open.pop();
                }
                break;
            }
            // �����ܱߵĵ�,�������
            {
                // ��ǰλ��
                auto const &ele = m_arr[curr.x][curr.y];
                /// �ֱ��ж��ĸ������Ƿ������
                //��
                {
                    // ����Ϊ
                    int nx = curr.x;
                    int ny = curr.y - 1;
                    if(!(nx < 0 || nx >= m_row || ny < 0 || ny >= m_col) && m_mark[nx][ny] == false) {
                        if(!ele.m_bLeft && !m_arr[nx][ny].m_bRight) {
                            q_open.push(CNode(nx, ny, 1 + curr.s, new CNode(curr)));
                            m_mark[nx][ny] = true;
                        }
                    }
                }
                //��
                {
                    // ����Ϊ
                    int nx = curr.x - 1;
                    int ny = curr.y;
                    if(!(nx < 0 || nx >= m_row || ny < 0 || ny >= m_col) && m_mark[nx][ny] == false) {
                        if(!ele.m_bTop && !m_arr[nx][ny].m_bBottom) {
                            q_open.push(CNode(nx, ny, 1 + curr.s, new CNode(curr)));
                            m_mark[nx][ny] = true;
                        }
                    }
                }
                //��
                {
                    // ����Ϊ
                    int nx = curr.x;
                    int ny = curr.y + 1;
                    if(!(nx < 0 || nx >= m_row || ny < 0 || ny >= m_col) && m_mark[nx][ny] == false) {
                        if(!ele.m_bRight && !m_arr[nx][ny].m_bLeft) {
                            q_open.push(CNode(nx, ny, 1 + curr.s, new CNode(curr)));
                            m_mark[nx][ny] = true;
                        }
                    }
                }
                //��
                {
                    // ����Ϊ
                    int nx = curr.x + 1;
                    int ny = curr.y;
                    if(!ele.m_bBottom && !m_arr[nx][ny].m_bTop) {
                        if(!(nx < 0 || nx >= m_row || ny < 0 || ny >= m_col) && m_mark[nx][ny] == false) {
                            q_open.push(CNode(nx, ny, 1 + curr.s, new CNode(curr)));
                            m_mark[nx][ny] = true;
                        }
                    }
                }
            }
        } // end while(!q_open.empty())

        return bFind;
    };

    // dfs �����������
    bool bfs(int x, int y, int m_targetX, int m_targetY, VPath &path)
    {
        // ��ʼ��������� : ��ʼȫ��δ�߹�
        m_mark.clear();
        for(int i = 0; i < m_row; ++i) {
            vector<bool> vLine(m_col, false);
            m_mark.push_back(vLine);
        }
        // ·�����
        m_path.clear();

        // ��ʼ��������� : ��ʼȫ��δ�߹�
        m_nodes.clear();
        for(int x = 0; x < m_row; ++x) {
            vector<CNode> vLine;
            for(int y = 0; y < m_row; ++y) {
                CNode node;
                node.x = x;
                node.y = y;
                node.s = 0;
                vLine.push_back(node);
            }
            m_nodes.push_back(vLine);
        }




        // ����㿪ʼ���� : ���������꼺����·���У���ֹ�����ظ���
        m_mark[x][y] = true;
        // ��ʼ �����������
        bool bFind = bfs_item(x, y);
        if(bFind) {
            std::copy(m_path.rbegin(), m_path.rend(), back_inserter<VPath>(path));
        }

        return bFind;
    };
    vector<vector<CNode>> m_nodes;
};



