//
//  Copyright (C) 2004-2011 by Autodesk, Inc.
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//

#ifndef _FS_WFS_GET_CAPABILITIES_H
#define _FS_WFS_GET_CAPABILITIES_H

class MgHttpWfsGetCapabilities : public MgHttpRequestResponseHandler, public IMgOgcDataAccessor
{
HTTP_DECLARE_CREATE_OBJECT()

public:
    /// <summary>
    /// Initializes the common parameters of the request.
    /// </summary>
    /// <param name="hRequest">Input
    /// This contains all the parameters of the request.
    /// </param>
    /// <returns>
    /// nothing
    /// </returns>
    MgHttpWfsGetCapabilities(MgHttpRequest *hRequest);

    /// <summary>
    /// Executes the specific request.
    /// </summary>
    /// <param name="hResponse">Input
    /// This contains the response (including MgHttpResult and StatusCode) from the server.
    /// </param>
    void Execute(MgHttpResponse& hResponse);

    /// <summary>
    /// Attempts to process a request made by POSTing an XML document
    /// </summary>
    /// <param name="hRequest">Input
    /// Contains the request params and data
    /// </param>
    /// <param name="hResponse">Input
    /// The response object into which to write the response data
    /// </param>
    /// <returns>
    /// True if this method can process this request type
    /// </returns>
    static bool ProcessPostRequest(MgHttpRequest *hRequest, MgHttpResponse& hResponse);

    /// <summary>
    /// Returns the classification of this request/response handler
    /// </summary>
    /// <returns>
    /// Classification of handler
    /// </returns>
    MgRequestClassification GetRequestClassification() { return MgHttpRequestResponseHandler::mrcWfs; }

    // IMgOgcDataAccessor method
    virtual void AcquireValidationData(MgOgcServer* ogcServer);

private:

    void InitializeRequestParameters(MgOgcWfsServer& oServer);
};

#endif  // _FS_WFS_GET_CAPABILITIES_H