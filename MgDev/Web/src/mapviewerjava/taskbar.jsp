<%--
  -Copyright (C) 2004-2008 by Autodesk, Inc.
  -This library is free software; you can redistribute it and/or
  -modify it under the terms of version 2.1 of the GNU Lesser
  -General Public License as published by the Free Software Foundation.

  -This library is distributed in the hope that it will be useful,
  -but WITHOUT ANY WARRANTY; without even the implied warranty of
  -MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  -Lesser General Public License for more details.

  -You should have received a copy of the GNU Lesser General Public
  -License along with this library; if not, write to the Free Software
  -Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
--%>

<%@ page contentType="text/html; charset=UTF-8" pageEncoding="UTF-8" %>
<%@ page import="org.osgeo.mapguide.*" %>
<%@ page import="java.util.*" %>
<%@ page import="java.io.*" %>
<%@ page import="java.text.*" %>
<%@ page import="javax.servlet.jsp.*" %>
<%@ page import="javax.servlet.http.*" %>
<%@ include file="common.jsp" %>

<%!
String locale;
%>

<%
    locale = "";
    response.setContentType("text/html; charset=UTF-8");
    request.setCharacterEncoding("UTF-8");

    MgLocalizer.SetLocalizedFilesPath(getServletContext().getRealPath("/") + "localized/");
    GetRequestParameters(request);

    response.getWriter().write(MgLocalizer.Localize(LoadTemplate("/viewerfiles/taskbar.templ"), locale, GetClientOS(request)));
%>

<%!
void GetRequestParameters(HttpServletRequest request)
{
    locale = GetParameter(request, "LOCALE");
}
%>
