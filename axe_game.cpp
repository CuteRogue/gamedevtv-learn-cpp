#include "raylib.h"
#include <cstdio>

int main()
{
    class Vector2Int
    {
        public: 
            int x;
            int y;
        
        Vector2Int()
        {
            x = 0;
            y = 0;
        }

        Vector2Int(int x, int y)
        {
            this->x = x;
            this->y = y;
        }

        Vector2Int operator+(const Vector2Int& value)
        {
            Vector2Int result;
            result.x = x + value.x;
            result.y = y + value.y;

            return result;
        }
        
        Vector2Int operator-(const Vector2Int& value) 
        {
            Vector2Int result;
            result.x = -value.x;
            result.y = -value.y;

            return result;
        }

        Vector2Int operator-() 
        {
            return Vector2Int(-this->x, -this->y);
        }

        Vector2Int operator+=(const Vector2Int& value) 
        { 
            *this = *this + value;
            return *this; 
        }

        Vector2Int operator-=(const Vector2Int& value) 
        { 
            *this = *this - value;
            return *this; 
        }
    };

    class BoxCollider
    {
        public:
            Vector2Int begin;
            Vector2Int end;
        
        BoxCollider(int xBegin, int yBegin, int xEnd, int yEnd)
        {
            begin = Vector2Int(xBegin, yBegin);
            end = Vector2Int(xEnd, yEnd);
        }

        BoxCollider(Vector2Int origin, Vector2Int size)
        {
            begin = Vector2Int(origin.x - size.x / 2, origin.y - size.y / 2);
            end = Vector2Int(origin.x + size.x / 2, origin.y + size.y / 2);
        }

        void DrawDebug(Color color)
        {
            DrawLine(begin.x, begin.y, end.x, begin.y, color);
            DrawLine(end.x, begin.y, end.x, end.y, color);
            DrawLine(end.x, end.y, begin.x, end.y, color);
            DrawLine(begin.x, end.y, begin.x, begin.y, color);
        }
    };

    Vector2Int windowSize = Vector2Int(800, 400);

    InitWindow(windowSize.x, windowSize.y, "Stupid Axe Game");

    Vector2Int circlePosition = Vector2Int(windowSize.x / 2, windowSize.y / 2);
    int circleRadius = 10;
    int circleMoveSpeed = 5;

    Vector2Int axeSize = Vector2Int(50, 50);
    Vector2Int axePosition = Vector2Int(windowSize.x - 200, 50);
    int axeGravity = 5;

    Vector2Int runtimeSpeedCircle;
    Vector2Int runtimeSpeedAxe = Vector2Int(0, axeGravity);

    bool collisionWithAxe = false;
    
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(SKYBLUE);

        if (collisionWithAxe)
        {
            DrawText("GAME OVER!!!", windowSize.x / 2, windowSize.y / 2, 36, RED);
        }
        else
        {
            runtimeSpeedCircle = Vector2Int(0,0);

            if (IsKeyDown(KEY_W) && circlePosition.y > (0 + circleRadius))
            { runtimeSpeedCircle -= Vector2Int(0, circleMoveSpeed); }
            if (IsKeyDown(KEY_S) && circlePosition.y < (windowSize.y - circleRadius))
            { runtimeSpeedCircle += Vector2Int(0, circleMoveSpeed); }
            if (IsKeyDown(KEY_A) && circlePosition.x > (0 + circleRadius))
            { runtimeSpeedCircle -= Vector2Int(circleMoveSpeed, 0); }
            if (IsKeyDown(KEY_D) && circlePosition.x < (windowSize.x - circleRadius))
            { runtimeSpeedCircle += Vector2Int(circleMoveSpeed, 0); }

            circlePosition += runtimeSpeedCircle;
            BoxCollider circleCollider = BoxCollider(circlePosition, Vector2Int(circleRadius * 2, circleRadius * 2));
            
            if (axePosition.y >= windowSize.y - axeSize.y || axePosition.y <= 0)
            {
                runtimeSpeedAxe = -runtimeSpeedAxe;
            }

            axePosition += runtimeSpeedAxe;
            BoxCollider axeCollider = BoxCollider(axePosition.x, axePosition.y, axePosition.x + axeSize.x, axePosition.y + axeSize.y);

            collisionWithAxe = axeCollider.end.y >= circleCollider.begin.y
                && axeCollider.begin.y <= circleCollider.end.y
                && axeCollider.begin.x <= circleCollider.end.x
                && axeCollider.end.x >= circleCollider.begin.x;

            DrawCircle(circlePosition.x, circlePosition.y, circleRadius, WHITE);
            circleCollider.DrawDebug(GREEN);
            
            DrawRectangle(axePosition.x, axePosition.y, axeSize.x, axeSize.y, RED);
            axeCollider.DrawDebug(GREEN);
        }

        EndDrawing();
    }
}