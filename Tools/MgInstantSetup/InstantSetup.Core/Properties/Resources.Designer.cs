﻿//------------------------------------------------------------------------------
// <auto-generated>
//     This code was generated by a tool.
//     Runtime Version:4.0.30319.18444
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
// </auto-generated>
//------------------------------------------------------------------------------

namespace InstantSetup.Core.Properties {
    using System;
    
    
    /// <summary>
    ///   A strongly-typed resource class, for looking up localized strings, etc.
    /// </summary>
    // This class was auto-generated by the StronglyTypedResourceBuilder
    // class via a tool like ResGen or Visual Studio.
    // To add or remove a member, edit your .ResX file then rerun ResGen
    // with the /str option, or rebuild your VS project.
    [global::System.CodeDom.Compiler.GeneratedCodeAttribute("System.Resources.Tools.StronglyTypedResourceBuilder", "4.0.0.0")]
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
    [global::System.Runtime.CompilerServices.CompilerGeneratedAttribute()]
    internal class Resources {
        
        private static global::System.Resources.ResourceManager resourceMan;
        
        private static global::System.Globalization.CultureInfo resourceCulture;
        
        [global::System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1811:AvoidUncalledPrivateCode")]
        internal Resources() {
        }
        
        /// <summary>
        ///   Returns the cached ResourceManager instance used by this class.
        /// </summary>
        [global::System.ComponentModel.EditorBrowsableAttribute(global::System.ComponentModel.EditorBrowsableState.Advanced)]
        internal static global::System.Resources.ResourceManager ResourceManager {
            get {
                if (object.ReferenceEquals(resourceMan, null)) {
                    global::System.Resources.ResourceManager temp = new global::System.Resources.ResourceManager("InstantSetup.Core.Properties.Resources", typeof(Resources).Assembly);
                    resourceMan = temp;
                }
                return resourceMan;
            }
        }
        
        /// <summary>
        ///   Overrides the current thread's CurrentUICulture property for all
        ///   resource lookups using this strongly typed resource class.
        /// </summary>
        [global::System.ComponentModel.EditorBrowsableAttribute(global::System.ComponentModel.EditorBrowsableState.Advanced)]
        internal static global::System.Globalization.CultureInfo Culture {
            get {
                return resourceCulture;
            }
            set {
                resourceCulture = value;
            }
        }
        
        /// <summary>
        ///   Looks up a localized string similar to SET MENTOR_DICTIONARY_PATH={0}
        ///pushd {1}
        ///httpd.exe
        ///popd.
        /// </summary>
        internal static string APACHE_WEB {
            get {
                return ResourceManager.GetString("APACHE_WEB", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Looks up a localized string similar to pushd {0}
        ///httpd.exe -k install -n &quot;{1}&quot;
        ///popd.
        /// </summary>
        internal static string APACHE_WEB_INSTALL {
            get {
                return ResourceManager.GetString("APACHE_WEB_INSTALL", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Looks up a localized string similar to pushd {0}
        ///httpd.exe
        ///popd.
        /// </summary>
        internal static string APACHE_WEB_NO_MENTOR {
            get {
                return ResourceManager.GetString("APACHE_WEB_NO_MENTOR", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Looks up a localized string similar to pushd {0}
        ///httpd.exe -k uninstall -n &quot;{1}&quot;
        ///popd.
        /// </summary>
        internal static string APACHE_WEB_UNINSTALL {
            get {
                return ResourceManager.GetString("APACHE_WEB_UNINSTALL", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Looks up a localized string similar to SET MENTOR_DICTIONARY_PATH={0}
        ///pushd {1}
        ///mgserver.exe run
        ///popd.
        /// </summary>
        internal static string SERVER {
            get {
                return ResourceManager.GetString("SERVER", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Looks up a localized string similar to SET MENTOR_DICTIONARY_PATH={0}
        ///pushd {1}
        ///mgserver install &quot;{2}&quot;
        ///popd.
        /// </summary>
        internal static string SERVER_INSTALL {
            get {
                return ResourceManager.GetString("SERVER_INSTALL", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Looks up a localized string similar to pushd {0}
        ///mgserver install &quot;{1}&quot;
        ///popd.
        /// </summary>
        internal static string SERVER_INSTALL_NO_MENTOR {
            get {
                return ResourceManager.GetString("SERVER_INSTALL_NO_MENTOR", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Looks up a localized string similar to pushd {0}
        ///mgserver.exe run
        ///popd.
        /// </summary>
        internal static string SERVER_NO_MENTOR {
            get {
                return ResourceManager.GetString("SERVER_NO_MENTOR", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Looks up a localized string similar to SET MENTOR_DICTIONARY_PATH={0}
        ///pushd {1}
        ///mgserver uninstall &quot;{2}&quot;
        ///popd.
        /// </summary>
        internal static string SERVER_UNINSTALL {
            get {
                return ResourceManager.GetString("SERVER_UNINSTALL", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Looks up a localized string similar to pushd {0}
        ///mgserver uninstall &quot;{1}&quot;
        ///popd.
        /// </summary>
        internal static string SERVER_UNINSTALL_NO_MENTOR {
            get {
                return ResourceManager.GetString("SERVER_UNINSTALL_NO_MENTOR", resourceCulture);
            }
        }
    }
}
