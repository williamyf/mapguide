<%--
Copyright (C) 2004-2006  Autodesk, Inc.
This library is free software; you can redistribute it and/or
modify it under the terms of version 2.1 of the GNU Lesser
General Public License as published by the Free Software Foundation.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
--%>

<%@ Page language="c#" %>
<%@ Import Namespace="System" %>
<%@ Import Namespace="System.IO" %>
<%@ Import Namespace="System.Collections.Specialized" %>
<%@ Import Namespace="System.Web" %>
<%@ Import Namespace="System.Globalization" %>
<%@ Import Namespace="OSGeo.MapGuide" %>

<!-- #Include File="common.aspx -->

<script runat="server">
String mapName = "";
String sessionId = "";
String mapFrame = "";
</script>

<%
    Response.Charset = "utf-8";

    try
    {
        //fetch the parameters for this request
        //
        GetRequestParameters();

        String vpath = GetSurroundVirtualPath(Request);
        String[] vals = {
            mapFrame,
            vpath + "legend.aspx",
            vpath + "legend.aspx",
            HttpUtility.UrlEncode(mapName),
            sessionId,
            vpath + "legendui.aspx"};

        String templ = LoadTemplate(Request, "../viewerfiles/legendctrl.templ");
        Response.Write(Substitute(templ, vals));
    }
    catch(Exception ne)
    {
        Response.Write(ne.Message);
    }
%>

<script runat="server">

void GetRequestParameters()
{
    if ("POST"== Request.HttpMethod)
    {
        GetParameters(Request.Form);
    }
    else
    {
        GetParameters(Request.QueryString);
    }
}

void GetParameters(NameValueCollection parameters)
{
    if(IsParameter(parameters, "MAPNAME"))
        mapName = GetParameter(parameters, "MAPNAME");

    if(IsParameter(parameters, "SESSION"))
        sessionId = GetParameter(parameters, "SESSION");

    if(IsParameter(parameters, "MAPFRAME"))
        mapFrame = GetParameter(parameters, "MAPFRAME");
    else
        mapFrame = "parent";
}

</script>
