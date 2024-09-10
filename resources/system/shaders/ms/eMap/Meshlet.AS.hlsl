#include "../Meshlet.hlsli"

[numthreads(1, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    PayLoad p;
    p.groupID = DTid.x;
    DispatchMesh(1, 1, 1, p);
}