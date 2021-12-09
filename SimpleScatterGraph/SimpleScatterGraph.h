#include<Windows.h>
#include "../Error/Errors.h"
#include "../CoOrdinateAxis/CoOrdinateAxis.h"
#include "../ScaleLines/ScaleLines.h"
#include "../GenericDCLL/GDCLL.h"
#include "../HetroTree/MemoryAllocator.h"

#define DOT_POINTS      1
#define SCATTER_SCALE   2

#define DEFAULT_GENESIS 30
#define DEFAULT_GAP 10

typedef unsigned long int HGRAPH;
typedef HGRAPH HDOTS;

typedef unsigned long int EditOption;
typedef int status_t;

typedef struct tagDISPLAYPOINTBUFFER
{
    POINT dot;
    NODE link;

}DISPLAYPOINTBUFFER,*LPDISPLAYPOINTBUFFER,*PDISPLAYPOINTBUFFER;

typedef struct tagDOTS
{
    LPPOINT Points;
    size_t size;

    bst_node_t treelink;
}DOTS,*LPDOTS,*PDOTS;

typedef struct tagSCATTERPLOT
{
    HDOTS hDot;
    HCOORDINATEAXIS hCoOdinateAxis;
    HSCALELINE hScaleLine;

    int genesis;
    int gap;

    LPCOLORREF pointColor;
    int scale;

    LPDCLL DisplayPointBuffer;
    bst_node_t treelink;

    size_t size;
}SCATTERPLOT,*LPSCATTERPLOT,*PSCATTERPLOT;

HDOTS CreateDots(LPPOINT lpPoints,size_t size);
LPDISPLAYPOINTBUFFER CreateDisplayPointBuffer(LONG x);

HGRAPH CreateScatterPlot(LPPOINT lpPoints,size_t size,COLORREF color,int scale);
status_t GetScatterPlot(HGRAPH hScalePlot,LPSCATTERPLOT lpScalePlot,EditOption option);

status_t SetDisplayPointBuffer(HGRAPH hScalePlot,size_t nSize);

status_t CALLBACK DestroyDOTS(bst_node_t* tree_link);
status_t CALLBACK DestroyScatterPlot(bst_node_t* tree_link);
status_t CALLBACK DestroyDisplayBuffer(LPNODE nodelink);