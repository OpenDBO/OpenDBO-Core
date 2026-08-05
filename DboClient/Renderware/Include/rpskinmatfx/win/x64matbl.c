/****************************************************************************
 *                                                                          *
 *  Module  :   x64matbl.c                                                  *
 *                                                                          *
 *  Purpose :   x64 C fallback for Intel SSE matrix-blending functions      *
 *                                                                          *
 ****************************************************************************/

#include "rpplugin.h"
#include "rwcore.h"
#include "rpdbgerr.h"
#include "rpskin.h"
#include "skin.h"
#include "ssematbl.h"

#define FLOATASINT(f) (*((const RwInt32 *)&(f)))

#define MATRIXPROCESSX(m, o, w) \
    ( ( (m)->right.x * (o).x + (m)->up.x * (o).y + (m)->at.x * (o).z + (m)->pos.x ) * (w) )
#define MATRIXPROCESSY(m, o, w) \
    ( ( (m)->right.y * (o).x + (m)->up.y * (o).y + (m)->at.y * (o).z + (m)->pos.y ) * (w) )
#define MATRIXPROCESSZ(m, o, w) \
    ( ( (m)->right.z * (o).x + (m)->up.z * (o).y + (m)->at.z * (o).z + (m)->pos.z ) * (w) )

#define MATRIXNORMALPROCESSX(m, o, w) \
    ( ( (m)->right.x * (o).x + (m)->up.x * (o).y + (m)->at.x * (o).z ) * (w) )
#define MATRIXNORMALPROCESSY(m, o, w) \
    ( ( (m)->right.y * (o).x + (m)->up.y * (o).y + (m)->at.y * (o).z ) * (w) )
#define MATRIXNORMALPROCESSZ(m, o, w) \
    ( ( (m)->right.z * (o).x + (m)->up.z * (o).y + (m)->at.z * (o).z ) * (w) )

/****************************************************************************
 _rpSkinIntelSSEMatrixBlend1Matrix

 x64 C fallback: transform vertices/normals by a single matrix.
 */
void
_rpSkinIntelSSEMatrixBlend1Matrix(RwInt32 numVertices,
                                  const RwMatrix *matrix,
                                  RwUInt8 *vertices,
                                  const RwV3d *originalVertices,
                                  RwUInt8 *normals,
                                  const RwV3d *originalNormals,
                                  RwUInt32 stride)
{
    RwInt32 i;

    RWFUNCTION(RWSTRING("_rpSkinIntelSSEMatrixBlend1Matrix"));

    for (i = 0; i < numVertices; i++)
    {
        RwV3d *vert = (RwV3d *)vertices;
        vert->x = MATRIXPROCESSX(matrix, originalVertices[i], 1.0f);
        vert->y = MATRIXPROCESSY(matrix, originalVertices[i], 1.0f);
        vert->z = MATRIXPROCESSZ(matrix, originalVertices[i], 1.0f);
        vertices += stride;

        if (normals != NULL)
        {
            RwV3d *norm = (RwV3d *)normals;
            norm->x = MATRIXNORMALPROCESSX(matrix, originalNormals[i], 1.0f);
            norm->y = MATRIXNORMALPROCESSY(matrix, originalNormals[i], 1.0f);
            norm->z = MATRIXNORMALPROCESSZ(matrix, originalNormals[i], 1.0f);
            normals += stride;
        }
    }

    RWRETURNVOID();
}

/****************************************************************************
 _rpSkinIntelSSEMatrixBlend1Weight

 x64 C fallback: each vertex has exactly 1 matrix index (weight=1.0 implied).
 */
void
_rpSkinIntelSSEMatrixBlend1Weight(RwInt32 numVertices,
                                  const RwUInt32 *matrixIndexMap,
                                  const RwMatrix *matrixArray,
                                  RwUInt8 *vertices,
                                  const RwV3d *originalVertices,
                                  RwUInt8 *normals,
                                  const RwV3d *originalNormals,
                                  RwUInt32 stride)
{
    RwInt32 i;

    RWFUNCTION(RWSTRING("_rpSkinIntelSSEMatrixBlend1Weight"));

    for (i = 0; i < numVertices; i++)
    {
        const RwMatrix *matrix = &matrixArray[matrixIndexMap[i] & 0xFF];
        RwV3d *vert = (RwV3d *)vertices;
        vert->x = MATRIXPROCESSX(matrix, originalVertices[i], 1.0f);
        vert->y = MATRIXPROCESSY(matrix, originalVertices[i], 1.0f);
        vert->z = MATRIXPROCESSZ(matrix, originalVertices[i], 1.0f);
        vertices += stride;

        if (normals != NULL)
        {
            RwV3d *norm = (RwV3d *)normals;
            norm->x = MATRIXNORMALPROCESSX(matrix, originalNormals[i], 1.0f);
            norm->y = MATRIXNORMALPROCESSY(matrix, originalNormals[i], 1.0f);
            norm->z = MATRIXNORMALPROCESSZ(matrix, originalNormals[i], 1.0f);
            normals += stride;
        }
    }

    RWRETURNVOID();
}

/****************************************************************************
 _rpSkinIntelSSEMatrixBlend2Weights

 x64 C fallback: optimized path for 2-weight skinning.
 */
void
_rpSkinIntelSSEMatrixBlend2Weights(RwInt32 numVertices,
                                   const RwMatrixWeights *matrixWeightsMap,
                                   const RwUInt32 *matrixIndexMap,
                                   const RwMatrix *matrixArray,
                                   RwUInt8 *vertices,
                                   const RwV3d *originalVertices,
                                   RwUInt8 *normals,
                                   const RwV3d *originalNormals,
                                   RwUInt32 stride)
{
    RwInt32 i;

    RWFUNCTION(RWSTRING("_rpSkinIntelSSEMatrixBlend2Weights"));

    for (i = 0; i < numVertices; i++)
    {
        const RwUInt32 matrixIndices = matrixIndexMap[i];
        const RwMatrixWeights *weights = &matrixWeightsMap[i];
        const RwMatrix *matrix;
        RwV3d *vert = (RwV3d *)vertices;
        RwReal vx = 0.0f, vy = 0.0f, vz = 0.0f;
        RwReal nx = 0.0f, ny = 0.0f, nz = 0.0f;

        if (FLOATASINT(weights->w0) > 0)
        {
            matrix = &matrixArray[matrixIndices & 0xFF];
            vx += MATRIXPROCESSX(matrix, originalVertices[i], weights->w0);
            vy += MATRIXPROCESSY(matrix, originalVertices[i], weights->w0);
            vz += MATRIXPROCESSZ(matrix, originalVertices[i], weights->w0);

            if (normals != NULL)
            {
                nx += MATRIXNORMALPROCESSX(matrix, originalNormals[i], weights->w0);
                ny += MATRIXNORMALPROCESSY(matrix, originalNormals[i], weights->w0);
                nz += MATRIXNORMALPROCESSZ(matrix, originalNormals[i], weights->w0);
            }
        }

        if (FLOATASINT(weights->w1) > 0)
        {
            matrix = &matrixArray[(matrixIndices >> 8) & 0xFF];
            vx += MATRIXPROCESSX(matrix, originalVertices[i], weights->w1);
            vy += MATRIXPROCESSY(matrix, originalVertices[i], weights->w1);
            vz += MATRIXPROCESSZ(matrix, originalVertices[i], weights->w1);

            if (normals != NULL)
            {
                nx += MATRIXNORMALPROCESSX(matrix, originalNormals[i], weights->w1);
                ny += MATRIXNORMALPROCESSY(matrix, originalNormals[i], weights->w1);
                nz += MATRIXNORMALPROCESSZ(matrix, originalNormals[i], weights->w1);
            }
        }

        vert->x = vx;
        vert->y = vy;
        vert->z = vz;
        vertices += stride;

        if (normals != NULL)
        {
            RwV3d *norm = (RwV3d *)normals;
            norm->x = nx;
            norm->y = ny;
            norm->z = nz;
            normals += stride;
        }
    }

    RWRETURNVOID();
}

/****************************************************************************
 _rpSkinIntelSSEMatrixBlend

 x64 C fallback: full 4-weight matrix blending.
 */
void
_rpSkinIntelSSEMatrixBlend(RwInt32 numVertices,
                            const RwMatrixWeights *matrixWeightsMap,
                            const RwUInt32 *matrixIndexMap,
                            const RwMatrix *matrixArray,
                            RwUInt8 *vertices,
                            const RwV3d *originalVertices,
                            RwUInt8 *normals,
                            const RwV3d *originalNormals,
                            RwUInt32 stride)
{
    RwInt32 i;

    RWFUNCTION(RWSTRING("_rpSkinIntelSSEMatrixBlend"));

    for (i = 0; i < numVertices; i++)
    {
        const RwUInt32 matrixIndices = matrixIndexMap[i];
        const RwMatrixWeights *weights = &matrixWeightsMap[i];
        const RwMatrix *matrix;
        RwV3d *vert = (RwV3d *)vertices;
        RwReal vx = 0.0f, vy = 0.0f, vz = 0.0f;
        RwReal nx = 0.0f, ny = 0.0f, nz = 0.0f;

        if (FLOATASINT(weights->w0) > 0)
        {
            matrix = &matrixArray[matrixIndices & 0xFF];
            vx += MATRIXPROCESSX(matrix, originalVertices[i], weights->w0);
            vy += MATRIXPROCESSY(matrix, originalVertices[i], weights->w0);
            vz += MATRIXPROCESSZ(matrix, originalVertices[i], weights->w0);
            if (normals != NULL)
            {
                nx += MATRIXNORMALPROCESSX(matrix, originalNormals[i], weights->w0);
                ny += MATRIXNORMALPROCESSY(matrix, originalNormals[i], weights->w0);
                nz += MATRIXNORMALPROCESSZ(matrix, originalNormals[i], weights->w0);
            }
        }

        if (FLOATASINT(weights->w1) > 0)
        {
            matrix = &matrixArray[(matrixIndices >> 8) & 0xFF];
            vx += MATRIXPROCESSX(matrix, originalVertices[i], weights->w1);
            vy += MATRIXPROCESSY(matrix, originalVertices[i], weights->w1);
            vz += MATRIXPROCESSZ(matrix, originalVertices[i], weights->w1);
            if (normals != NULL)
            {
                nx += MATRIXNORMALPROCESSX(matrix, originalNormals[i], weights->w1);
                ny += MATRIXNORMALPROCESSY(matrix, originalNormals[i], weights->w1);
                nz += MATRIXNORMALPROCESSZ(matrix, originalNormals[i], weights->w1);
            }
        }

        if (FLOATASINT(weights->w2) > 0)
        {
            matrix = &matrixArray[(matrixIndices >> 16) & 0xFF];
            vx += MATRIXPROCESSX(matrix, originalVertices[i], weights->w2);
            vy += MATRIXPROCESSY(matrix, originalVertices[i], weights->w2);
            vz += MATRIXPROCESSZ(matrix, originalVertices[i], weights->w2);
            if (normals != NULL)
            {
                nx += MATRIXNORMALPROCESSX(matrix, originalNormals[i], weights->w2);
                ny += MATRIXNORMALPROCESSY(matrix, originalNormals[i], weights->w2);
                nz += MATRIXNORMALPROCESSZ(matrix, originalNormals[i], weights->w2);
            }
        }

        if (FLOATASINT(weights->w3) > 0)
        {
            matrix = &matrixArray[(matrixIndices >> 24) & 0xFF];
            vx += MATRIXPROCESSX(matrix, originalVertices[i], weights->w3);
            vy += MATRIXPROCESSY(matrix, originalVertices[i], weights->w3);
            vz += MATRIXPROCESSZ(matrix, originalVertices[i], weights->w3);
            if (normals != NULL)
            {
                nx += MATRIXNORMALPROCESSX(matrix, originalNormals[i], weights->w3);
                ny += MATRIXNORMALPROCESSY(matrix, originalNormals[i], weights->w3);
                nz += MATRIXNORMALPROCESSZ(matrix, originalNormals[i], weights->w3);
            }
        }

        vert->x = vx;
        vert->y = vy;
        vert->z = vz;
        vertices += stride;

        if (normals != NULL)
        {
            RwV3d *norm = (RwV3d *)normals;
            norm->x = nx;
            norm->y = ny;
            norm->z = nz;
            normals += stride;
        }
    }

    RWRETURNVOID();
}
