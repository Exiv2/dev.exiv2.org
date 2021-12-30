// ***************************************************************** -*- C++ -*-
/*
 * Copyright (C) 2004, 2005 Brad Schick <brad@robotbattle.com>
 * 
 * This program is part of the PSAUtils distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */
/*
  File:      metadata.cs
  Version:   $Rev$
  Author(s): Brad Schick <brad@robotbattle.com>
  History:   12-Nov-04, brad: created
 */
// *****************************************************************************


using System;
using System.ComponentModel;
using System.Collections;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Text;


namespace PSAUtils
{
    /// <summary>
    /// Summary description for MetaData.
    /// This object is not thread safe.
    /// Users of this class should call the Dispose() method
    /// when complete to free unmanaged resources.
    /// </summary>
    public class MetaData : System.ComponentModel.Component
    {
        // Data
        private IntPtr image;
        private string imageName;
        private Object userData;
        private EnumMetaDelegate userDelegate;

        // Public constants
        public enum TypeId  {
            invalidTypeId, unsignedByte, asciiString, unsignedShort, 
            unsignedLong, unsignedRational, invalid6, undefined, 
            signedShort, signedLong, signedRational, 
            stringId, date, time, lastTypeId 
        };


        // Public methods
        public delegate bool EnumMetaDelegate( string key, string val, Object user );

        public void EnumMeta(EnumMetaDelegate del, Object data)
        {
            userData = data;
            userDelegate = del;
            EnumMetaDelegateExt localDel = new EnumMetaDelegateExt( EnumProcMapper );
            EnumMetaExt( image, localDel, IntPtr.Zero );
        }

        // If not called all changes are thrown out
        public void SaveChanges( )
        {
            if( SaveImageExt(image) != 0 )
                throw new ApplicationException( "Could not save image: " + imageName );
        }

        public string ReadMeta( string key )
        {
            // limit of 2k for now
            StringBuilder buff = new StringBuilder(2048);
            if( ReadMetaExt(image, key, buff, buff.Capacity) != 0 )
                throw new ApplicationException( "Could not read " + key + " in image: "  + imageName );

            return buff.ToString();
        }

        public void ModifyMeta( string key, string val )
        {
            // invalidTypeId causes type guess
            ModifyMeta( key, val, TypeId.invalidTypeId );
        }

        public void ModifyMeta( string key, string val, TypeId type )
        {
            if( ModifyMetaExt(image, key, val, (int)type) != 0)
                throw new ApplicationException( "Could not modify " + key + " in image: "  + imageName );
        }

        public void AddMeta( string key, string val )
        {
            // invalidTypeId causes type guess
            AddMeta( key, val, TypeId.invalidTypeId );
        }

        public void AddMeta( string key, string val, TypeId type )
        {
            if( AddMetaExt(image, key, val, (int)type) != 0)
                throw new ApplicationException( "Could not add " + key + " to image: "  + imageName );
        }

        public void RemoveMeta( string key )
        {
            if( RemoveMetaExt(image, key) != 0)
                throw new ApplicationException( "Could not remove " + key + " from image: "  + imageName );
        }

    
        /// <summary>
        /// exivsimple.dll imports
        /// </summary>
        [DllImport("exivsimple.dll", EntryPoint="OpenFileImage")]
        private static extern IntPtr OpenFileImageExt(string fileName);

        [DllImport("exivsimple.dll", EntryPoint="OpenMemImage")]
        private static extern IntPtr OpenMemImageExt( [Out] byte[] data, int size);

        [DllImport("exivsimple.dll", EntryPoint="FreeImage")]
        private static extern void FreeImageExt(IntPtr img);

        private delegate bool EnumMetaDelegateExt( string key, string val, IntPtr user );

        [DllImport("exivsimple.dll", EntryPoint="EnumMeta")]
        private static extern void EnumMetaExt(IntPtr img, EnumMetaDelegateExt del, IntPtr user);

        [DllImport("exivsimple.dll", EntryPoint="SaveImage")]
        private static extern int SaveImageExt(IntPtr img);

        [DllImport("exivsimple.dll", EntryPoint="ImageSize")]
        private static extern int ImageSizeExt(IntPtr img);

        [DllImport("exivsimple.dll", EntryPoint="ImageData")]
        private static extern int ImageDataExt(IntPtr img, [In] byte[] data, int size);

        [DllImport("exivsimple.dll", EntryPoint="ReadMeta")]
        private static extern int ReadMetaExt(IntPtr img, string key, StringBuilder buff, int buffsize);

        [DllImport("exivsimple.dll", EntryPoint="ModifyMeta")]
        private static extern int ModifyMetaExt(IntPtr img, string key, string val, int type);

        [DllImport("exivsimple.dll", EntryPoint="AddMeta")]
        private static extern int AddMetaExt(IntPtr img, string key, string val, int type);

        [DllImport("exivsimple.dll", EntryPoint="RemoveMeta")]
        private static extern int RemoveMetaExt(IntPtr img, string key);

        private bool EnumProcMapper( string key, string val, IntPtr user )
        {
            return userDelegate( key, val, userData );
        }

        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.Container components = null;

        public MetaData(System.ComponentModel.IContainer container, string file)
        {
            ///
            /// Required for Windows.Forms Class Composition Designer support
            ///
            container.Add(this);
            InitializeComponent();

            this.imageName = file;
            image = OpenFileImageExt( file );
            if( image == IntPtr.Zero )
                throw new ApplicationException( "Could not open file: " + file );
        }

        public MetaData( string file )
        {
            ///
            /// Required for Windows.Forms Class Composition Designer support
            ///
            InitializeComponent();

            this.imageName = file;
            image = OpenFileImageExt( file );
            if( image == IntPtr.Zero )
                throw new ApplicationException( "Could not open file: " + file );
        }

        public MetaData( byte[] data )
        {
            ///
            /// Required for Windows.Forms Class Composition Designer support
            ///
            InitializeComponent();

            this.imageName = "In Memory";
            image = OpenMemImageExt( data, data.Length );
            if( image == IntPtr.Zero )
                throw new ApplicationException( "Could not open in memory image" );
        }

        public byte[] ImageData()
        {
            int size = ImageSizeExt( image );

            if( size <= 0 )
                throw new ApplicationException( "Could not get image size" );

            byte[] data = new byte[size];
            if( ImageDataExt( image, data, size ) > 0 )
                return data;
            else
                throw new ApplicationException( "Could not read image data" );
        }

        /// <summary> 
        /// Clean up any resources being used.
        /// </summary>
        protected override void Dispose( bool disposing )
        {
            try
            {
                if( disposing )
                {
                    if(components != null)
                    {
                        components.Dispose();
                    }
                }

                // Release unmanaged resources. If disposing is false, 
                // only the following code is executed.
                if( image != IntPtr.Zero )
                {
                    FreeImageExt( image );
                    image = IntPtr.Zero;
                }
            }
            finally
            {
                base.Dispose( disposing );
            }
        }


        #region Component Designer generated code
        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            components = new System.ComponentModel.Container();
        }
        #endregion
    }
}
