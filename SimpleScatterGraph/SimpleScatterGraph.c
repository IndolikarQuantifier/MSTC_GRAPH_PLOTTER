#include "SimpleScatterGraph.h"


HDOTS CreateDots(LPPOINT lpPoints,size_t size)
{
    LPDOTS lpDots =  NULL;
    key_t dotKey;
    GraphResourceAlloc(DOTS,treelink,sizeof(DOTS),lpDots,dotKey,DestroyDOTS,NULL);

    CHECK_ERROR(lpDots == NULL,
            "Not able to allocate Memory !!!",
            "CreateDots()"
    );

    lpDots->Points = lpPoints;
    lpDots->size = size;

    return dotKey;
}

LPDISPLAYPOINTBUFFER CreateDisplayPointBuffer(LONG x)
{
    LPDISPLAYPOINTBUFFER newDisplayPointBuffer = NULL;
    newDisplayPointBuffer = (LPDISPLAYPOINTBUFFER) xcalloc(sizeof(DISPLAYPOINTBUFFER));

    newDisplayPointBuffer->dot.x = x;

    return newDisplayPointBuffer;
}

HGRAPH CreateScatterPlot(LPPOINT lpPoints,size_t size,COLORREF color,int scale)
{
    HDOTS newDots = CreateDots(lpPoints,size);
    HCOORDINATEAXIS newCoOrdinate = CreateCoOrdinateAxis();
    HSCALELINE newScaleLine = CreateScaleLines();

    LPSCATTERPLOT lpScatterPlot = NULL;
    key_t scatterKey;

    GraphResourceAlloc(SCATTERPLOT,treelink,sizeof(SCATTERPLOT),lpScatterPlot,scatterKey,DestroyScatterPlot,NULL);

    CHECK_ERROR(
        lpScatterPlot == NULL,
        "Not able to allocate Memory !!!",
        "CreateScatterPlot()"
    );

    lpScatterPlot->hDot = newDots;
    lpScatterPlot->hCoOdinateAxis = newCoOrdinate;
    lpScatterPlot->hScaleLine = newScaleLine;

    lpScatterPlot->genesis = DEFAULT_GENESIS;
    lpScatterPlot->gap = DEFAULT_GAP;

    lpScatterPlot->size = size;
    lpScatterPlot->pointColor = color;
    lpScatterPlot->scale = scale;
    lpScatterPlot->DisplayPointBuffer = CreateDCLL();

    return scatterKey;
}

status_t GetScatterPlot(HGRAPH hScalePlot,LPSCATTERPLOT lpScalePlot,EditOption option)
{
    bst_node_t* treelink = NULL;
    LPSCATTERPLOT lpScatterPlot = NULL;

    GetGraphResource(hScalePlot,&treelink);
    lpScalePlot = CONTAINER_OF(treelink,SCATTERPLOT,treelink);

    if(option | SCATTER_SCALE)
    {
        lpScalePlot->scale = lpScalePlot->scale;
    }

    return SUCCESS;
}

status_t SetDisplayPointBuffer(HGRAPH hScalePlot,size_t nSize)
{
    LPSCATTERPLOT lpScatterPlot = NULL;
    bst_node_t* treelink = NULL;

    GetGraphResource(hScalePlot,&treelink);
    lpScatterPlot = CONTAINER_OF(treelink,SCATTERPLOT,treelink);

    int genesis = lpScatterPlot->genesis;
    int gap = lpScatterPlot->gap;

    for(size_t i=lpScatterPlot->DisplayPointBuffer->nr_elements;i<nSize;i++)
    {
        LPDISPLAYPOINTBUFFER newDisplayBuffer = NULL;
        LONG x = genesis + (gap * i);
        newDisplayBuffer = CreateDisplayPointBuffer(x);

        InsertEnd(lpScatterPlot->DisplayPointBuffer,&(newDisplayBuffer->link));
    }

    return SUCCESS;

}

status_t CALLBACK DestroyDOTS(bst_node_t* tree_link)
{
    LPDOTS lpDots = NULL;
    lpDots = CONTAINER_OF(tree_link,DOTS,treelink);

    free(lpDots->Points);
    lpDots->Points = NULL;
    free(lpDots);
    lpDots = NULL;

    return SUCCESS;
}

status_t CALLBACK DestroyScatterPlot(bst_node_t* tree_link)
{
    LPSCATTERPLOT lpScatterPlot = NULL;
    lpScatterPlot = CONTAINER_OF(tree_link,SCATTERPLOT,treelink);

    DeAllocate(lpScatterPlot->hDot);
    DeAllocate(lpScatterPlot->hCoOdinateAxis);
    DeAllocate(lpScatterPlot->hScaleLine);

    DestoryDCLL(lpScatterPlot->DisplayPointBuffer,DestroyDisplayBuffer);

    free(lpScatterPlot);

    return SUCCESS;
}

status_t CALLBACK DestroyDisplayBuffer(LPNODE nodelink)
{
    LPDISPLAYPOINTBUFFER lpDisplayPointBuffer = NULL;

    lpDisplayPointBuffer = CONTAINER_OF(nodelink,DISPLAYPOINTBUFFER,link);

    free(lpDisplayPointBuffer);

    return SUCCESS;
}