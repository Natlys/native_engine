#include "nw_app_pch.hpp"

namespace NW
{
    ////////////////////////////////////////////////////////////////////////
    // CircumCircle() :
    //   Return true if a point (xp,yp) is inside the circumcircle made up
    //   of the points (x1,y1), (x2,y2), (x3,y3)
    //   The circumcircle centre is returned in (xc,yc) and the radius r
    //   Note : A point on the edge is inside the circumcircle
    ////////////////////////////////////////////////////////////////////////
    inline v1bit_t circum_circle(v2f_t point, v2f_t xy1, v2f_t xy2, v2f_t xy3, v3f_t& circle_xyr) {
        v1f_t m1, m2, mx1, mx2, my1, my2;
        v1f_t dx, dy, rsqr, drsqr;
        v3f_t result;
        // check for coincident points;
        if (abs(xy1[1] - xy2[1]) < 0.00001f && abs(xy2[1] - xy3[1]) < 0.00001f) { return NW_FALSE; }
        if (abs(xy2[1] - xy1[1]) < 0.00001f) {
            m2 = -(xy3[0] - xy2[0]) / (xy3[1] - xy2[1]);
            mx2 = (xy2[0] + xy3[0]) / 2.0;
            my2 = (xy2[1] + xy3[1]) / 2.0;
            circle_xyr[0] = (xy2[0] + xy1[0]) / 2.0;
            circle_xyr[1] = m2 * (circle_xyr[0] - mx2) + my2;
        }
        else if (abs(xy3[1] - xy2[1]) < 0.00001f) {
            m1 = -(xy2[0] - xy1[0]) / (xy2[1] - xy1[1]);
            mx1 = (xy1[0] + xy2[0]) / 2.0;
            my1 = (xy1[1] + xy2[1]) / 2.0;
            circle_xyr[0] = (xy3[0] + xy2[0]) / 2.0;
            circle_xyr[1] = m1 * (circle_xyr[0] - mx1) + my1;
        }
        else {
            m1 = -(xy2[0] - xy1[0]) / (xy2[1] - xy1[1]);
            m2 = -(xy3[0] - xy2[0]) / (xy3[1] - xy2[1]);
            mx1 = (xy1[0] + xy2[0]) / 2.0;
            mx2 = (xy2[0] + xy3[0]) / 2.0;
            my1 = (xy1[1] + xy2[1]) / 2.0;
            my2 = (xy2[1] + xy3[1]) / 2.0;
            circle_xyr[0] = (m1 * mx1 - m2 * mx2 + my2 - my1) / (m1 - m2);
            circle_xyr[1] = m1 * (circle_xyr[0] - mx1) + my1;
        }
        dx = xy2[0] - circle_xyr[0];
        dy = xy2[1] - circle_xyr[1];
        rsqr = dx * dx + dy * dy;
        circle_xyr[2] = sqrt(rsqr);
        dx = point[0] - circle_xyr[0];
        dy = point[1] - circle_xyr[1];
        drsqr = dx * dx + dy * dy;
        return drsqr <= rsqr;
    }
    ///////////////////////////////////////////////////////////////////////////////
    // Triangulate() :
    //   triangulation subroutine
    //   takes as input a buffer of vertices with specified size and type
    //   constructs index and vertex data for triangles written into given buffers
    //   these triangles are arranged in a consistent clockwise order;
    //   the triangle index array should be malloced to 3 * count_of_vbuf
    //   the vertex array vbuf must be big enough to hold 3 more points
    //   the vertex array must be sorted in increasing x values say
    //
    //   qsort(p, nv, sizeof(XYZ), XYZCompare);
    ///////////////////////////////////////////////////////////////////////////////
    inline v1bit_t triangulate(mem_buf_t& vbuf, mem_buf_t& ibuf) {
        NW_CHECK(vbuf.get_layt().has_type<v2f_t>(), "type error!", return NW_FALSE);
        NW_CHECK(ibuf.get_layt().has_type<v3u>(), "type error!", return NW_FALSE);

        array_t<v1bit_t> complete;
        array_t<v2u> ibuf_edges;
        v1s_t nedge = NW_NULL;
        v1bit_t status = NW_FALSE;
        v1bit_t is_inside = NW_FALSE;

        v2f_t point;
        v2f_t xy1, xy2, xy3;
        v3f_t circle_xyr;
        
        v1f_t xmin, xmax, ymin, ymax, xmid, ymid;
        v1f_t dx, dy, dmax;

        // allocate memory for the completeness list, flag for each triangle;
        v1s_t trimax = trimax = 4 * ibuf.get_count();
        complete.resize(trimax);
        // allocate memory for the edge list;
        v1s_t emax = trimax;
        ibuf_edges.resize(emax);
        // ffind the maximum and minimum vertex bounds;
        // this is to allow calculation of the bounding triangle
        xmin = vbuf[0u].get<v2f_t>()[0];
        ymin = vbuf[0u].get<v2f_t>()[1];
        xmax = xmin;
        ymax = ymin;
        for (v1u_t i = 1; i < vbuf.get_count(); i++) {
            if (vbuf[i].get<v2f_t>()[0] < xmin) { xmin = vbuf[i].get<v2f_t>()[0]; }
            if (vbuf[i].get<v2f_t>()[0] > xmax) { xmax = vbuf[i].get<v2f_t>()[0]; }
            if (vbuf[i].get<v2f_t>()[1] < ymin) { ymin = vbuf[i].get<v2f_t>()[1]; }
            if (vbuf[i].get<v2f_t>()[1] > ymax) { ymax = vbuf[i].get<v2f_t>()[1]; }
        }
        dx = xmax - xmin;
        dy = ymax - ymin;
        dmax = (dx > dy) ? dx : dy;
        xmid = (xmax + xmin) / 2.0f;
        ymid = (ymax + ymin) / 2.0f;
        // the supertriangle which encompasses all the sample points;
        // the supertriangle coordinates are added to the end of the vertex list;
        // the supertriangle is the first triangle in the triangle list;
        vbuf[vbuf.get_count() - 3].get<v2f_t>()[0] = xmid - 20 * dmax;
        vbuf[vbuf.get_count() - 3].get<v2f_t>()[1] = ymid - dmax;
        vbuf[vbuf.get_count() - 2].get<v2f_t>()[0] = xmid;
        vbuf[vbuf.get_count() - 2].get<v2f_t>()[1] = ymid + 20 * dmax;
        vbuf[vbuf.get_count() - 1].get<v2f_t>()[0] = xmid + 20 * dmax;
        vbuf[vbuf.get_count() - 1].get<v2f_t>()[1] = ymid - dmax;
        ibuf[0u].get<v3u>()[0] = ibuf.get_count();
        ibuf[0u].get<v3u>()[1] = ibuf.get_count() + 1;
        ibuf[0u].get<v3u>()[2] = ibuf.get_count() + 2;
        complete[0] = NW_FALSE;
        // include each point one at a time into the existing mesh
        v1u_t vcount = vbuf.get_count();
        v1u_t icount = 0u;
        for (v1u_t i = 0; i < vbuf.get_count(); i++) {
            point[0] = vbuf[i].get<v2f_t>()[0];
            point[1] = vbuf[i].get<v2f_t>()[1];
            nedge = 0;
            // set up the edge buffer.
            // if the point (xp,yp) lies inside the circumcircle then the
            // three edges of that triangle are added to the edge buffer
            // and that triangle is removed;
            for (v1u_t j = 0; j < icount; j++) {
                if (complete[j]) { continue; }
                xy1[0] = vbuf[ibuf[j].get<v3u>()[0]].get<v2f_t>()[0]; xy1[1] = vbuf[ibuf[j].get<v3u>()[0]].get<v2f_t>()[1];
                xy2[0] = vbuf[ibuf[j].get<v3u>()[1]].get<v2f_t>()[0]; xy2[1] = vbuf[ibuf[j].get<v3u>()[1]].get<v2f_t>()[1];
                xy3[0] = vbuf[ibuf[j].get<v3u>()[2]].get<v2f_t>()[0]; xy3[1] = vbuf[ibuf[j].get<v3u>()[2]].get<v2f_t>()[1];
                is_inside = circum_circle(point, xy1, xy2, xy3, circle_xyr);
                if (circle_xyr[0] + circle_xyr[2] < point[0]) { // suggested if (xc + r + EPSILON < xp)
                    complete[j] = NW_TRUE;
                }
                if (is_inside) {
                    // check that we haven't exceeded the edge list size
                    if (nedge + 3 >= emax) { emax += 100; }
                    ibuf_edges.resize(emax);
                    ibuf_edges[nedge + 0][0] = ibuf[j].get<v3u>()[0];
                    ibuf_edges[nedge + 0][1] = ibuf[j].get<v3u>()[1];
                    ibuf_edges[nedge + 1][0] = ibuf[j].get<v3u>()[1];
                    ibuf_edges[nedge + 1][1] = ibuf[j].get<v3u>()[2];
                    ibuf_edges[nedge + 2][0] = ibuf[j].get<v3u>()[2];
                    ibuf_edges[nedge + 2][1] = ibuf[j].get<v3u>()[0];
                    nedge += 3;
                    ibuf[j].get<v3u>() = ibuf[icount - 1].get<v3u>();
                    complete[j] = complete[icount - 1];
                    icount--;
                    j--;
                }
            }
            // tag multiple edges
            // note: if all triangles are specified anticlockwise then all
            // interior edges are opposite pointing in direction;
            for (v1s_t j = 0; j < nedge - 1; j++) {
                for (v1s_t k = j + 1; k < nedge; k++) {
                    if ((ibuf_edges[j][0] == ibuf_edges[k][1]) && (ibuf_edges[j][1] == ibuf_edges[k][0])) {
                        ibuf_edges[j][0] = -1;
                        ibuf_edges[j][1] = -1;
                        ibuf_edges[k][0] = -1;
                        ibuf_edges[k][1] = -1;
                    }
                    // shouldn't need the following, see note above
                    if ((ibuf_edges[j][0] == ibuf_edges[k][0]) && (ibuf_edges[j][1] == ibuf_edges[k][1])) {
                        ibuf_edges[j][0] = -1;
                        ibuf_edges[j][1] = -1;
                        ibuf_edges[k][0] = -1;
                        ibuf_edges[k][1] = -1;
                    }
                }
            }
            // form new triangles for the current point
            // skipping over any tagged edges;
            // all edges are arranged in clockwise order;
            for (v1s_t j = 0; j < nedge; j++) {
                if (ibuf_edges[j][0] < 0 || ibuf_edges[j][1] < 0) { continue; }
                ibuf[icount].get<v3u>()[0] = ibuf[j].get<v3u>()[0];
                ibuf[icount].get<v3u>()[1] = ibuf[j].get<v3u>()[1];
                ibuf[icount].get<v3u>()[2] = i;
                complete[icount] = NW_FALSE;
                icount++;
            }
        }
        // remove triangles with supertriangle vertices
        // these are triangles which have a vertex number greater than nv
        for (v1s_t i = 0; i < icount; i++) {
            if (ibuf[i].get<v3u>()[0] >= vcount || ibuf[i].get<v3u>()[1] >= vcount || ibuf[i].get<v3u>()[2] >= vcount) {
                ibuf[i].get<v3u>() = ibuf[icount - 1].get<v3u>();
                icount--;
                i--;
            }
        }
        return NW_TRUE;
    }
    
    inline v1s_t xyz_cmp(ptr_tc v1, ptr_tc v2) {
        v3u& xyz1 = *((v3u*)v1);
        v3u& xyz2 = *((v3u*)v2);
        if (xyz1[0] < xyz2[0]) { return -1; }
        else if (xyz1[0] > xyz2[0]) { return 1; }
        else { return 0; }
    }
}