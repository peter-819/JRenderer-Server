using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.Serialization;
using System.Runtime.Serialization.Formatters.Binary;
using System.Text;
using System.Threading.Tasks;

namespace JRenderer_Server
{
    sealed class ClientToServerDeserializationBinder : SerializationBinder
    {
        public override Type BindToType(string assemblyName, string typeName)
        {
            if (typeName == "JRenderer_Client.PpmImage")//ProjectSend序列化时的命名空间，SendClass序列化时的类 
            {
                typeName = "JRenderer_Server.PpmImage";//ProjectReceive反序列化时的命名空间，ReceiveClass反序列化时的类
            }
            return Type.GetType(typeName, true);
        }
    }
    sealed class ServerToClientDeserializationBinder : SerializationBinder
    {
        public override Type BindToType(string assemblyName, string typeName)
        {
            if (typeName == "JRenderer_Server.PpmImage")//ProjectSend序列化时的命名空间，SendClass序列化时的类 
            {
                typeName = "JRenderer_Client.PpmImage";//ProjectReceive反序列化时的命名空间，ReceiveClass反序列化时的类
            }
            return Type.GetType(typeName, true);
        }
    }
    [Serializable]
    class PpmImage
    {
        public int width { get; set; }
        public int height { get; set; }
        public int colorRange { get; set; }
        public byte[] rgb;
        
        public static byte[] Serialize(object obj)
        {
            if (obj == null)
                return null;
            //内存实例
            MemoryStream ms = new MemoryStream();
            //创建序列化的实例
            BinaryFormatter formatter = new BinaryFormatter();
            long size = ms.GetBuffer().Length;
            formatter.Serialize(ms, obj);//序列化对象，写入ms流中  
            ms.Position = 0;
            //byte[] bytes = new byte[ms.Length];//这个有错误
            byte[] bytes = ms.GetBuffer();
            ms.Read(bytes, 0, bytes.Length);
            ms.Close();
            return bytes;
        }
        public static PpmImage Deserialize(byte[] bytes)
        {
            object obj = null;
            if (bytes == null)
                return null;
            //利用传来的byte[]创建一个内存流
            MemoryStream ms = new MemoryStream(bytes);
            ms.Position = 0;
            BinaryFormatter formatter = new BinaryFormatter();
            formatter.Binder = new ClientToServerDeserializationBinder();
            obj = formatter.Deserialize(ms);//把内存流反序列成对象  
            ms.Close();
            return obj as PpmImage;
        }
    }
    class PpmHelper
    {
        static public PpmImage ReadPpmFile(string fileName)
        {
            string context = File.ReadAllText(fileName);
            context = context.Replace("\n", " ");
            context = context.Replace("\r", "");
            string[] datas = context.Split(' ');
            var image = new PpmImage();
            if (datas[0] == "P3")
            {
                image.width = int.Parse(datas[1]);
                image.height = int.Parse(datas[2]);
                image.colorRange = int.Parse(datas[3]);
                var pixels = image.width * image.height;
                image.rgb = new byte[pixels*3];
                for(int i = 0; i < pixels * 3; i++)
                {
                    image.rgb[i] = byte.Parse(datas[i + 4]);
                }
            }
            else
            {
                throw new Exception("PPM file is not ASCII format");
            }
            return image;
        }
    }
}
