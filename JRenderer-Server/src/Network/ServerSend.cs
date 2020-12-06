using JRenderer_Server;
using System.Collections;
using System.Collections.Generic;
using System.Xml.Serialization;
/// <summary>
/// 向客户端发送数据
/// </summary>
public class ServerSend 
{
    /// <summary>
    /// 向toClient发送TCP数据
    /// </summary>
    private static void SendTCPData(int _toClient, Packet _packet)
    {
        _packet.WriteLength();
        Server.clients[_toClient].tcp.SendData(_packet);
    }
    /// <summary>
    /// 向toClient发送UDP数据
    /// </summary>
    private static void SendUDPData(int _toClient, Packet _packet)
    {
        _packet.WriteLength();
        Server.clients[_toClient].udp.SendData(_packet);
    }
    /// <summary>
    /// 向所有用户发送TCP数据
    /// </summary>
    private static void SendTCPDataToAll(Packet _packet)
    {
        _packet.WriteLength();
        for (int i = 1; i <= Server.MaxPlayers; i++)
        {
            Server.clients[i].tcp.SendData(_packet);
        }
    }
    /// <summary>
    /// 向除ExceptClient的所有用户发送TCP数据
    /// </summary>
    private static void SendTCPDataToAll(int _exceptClient, Packet _packet)
    {
        _packet.WriteLength();
        for (int i = 1; i <= Server.MaxPlayers; i++)
        {
            if (i != _exceptClient)
            {
                Server.clients[i].tcp.SendData(_packet);
            }
        }
    }
    /// <summary>
    /// 向所有用户发送UDP数据
    /// </summary>
    private static void SendUDPDataToAll(Packet _packet)
    {
        _packet.WriteLength();
        for (int i = 1; i <= Server.MaxPlayers; i++)
        {
            Server.clients[i].udp.SendData(_packet);
        }
    }
    /// <summary>
    /// 向除ExceptClient的所有用户发送UDP数据
    /// </summary>
    private static void SendUDPDataToAll(int _exceptClient, Packet _packet)
    {
        _packet.WriteLength();
        for (int i = 1; i <= Server.MaxPlayers; i++)
        {
            if (i != _exceptClient)
            {
                Server.clients[i].udp.SendData(_packet);
            }
        }
    }
    /// <summary>
    /// 向toClient用户发送Welcome信息
    /// </summary>
    /// <param name="_toClient"></param>
    /// <param name="_msg"></param>
    public static void Welcome(int _toClient, string _msg)
    {
        using (Packet _packet = new Packet((int)ServerPackets.welcome))
        {
            _packet.Write(_msg);
            _packet.Write(_toClient);
            SendTCPData(_toClient, _packet);
        }
    }
    
    public static void SendPpmTest(int _toClient)
    {
        PpmImage image = PpmHelper.ReadPpmFile("test.ppm");
        using (Packet _packet = new Packet((int)ServerPackets.ppmFile))
        {
            byte[] data = PpmImage.Serialize(image);
            _packet.Write(data.Length);
            _packet.Write(data);
            SendUDPData(_toClient, _packet);
        }
    }
    public static void sendLoginResult(int _toClient, bool res)
    {
        using(Packet _packet = new Packet((int)ServerPackets.LoginResult))
        {
            _packet.Write(res);
            SendTCPData(_toClient ,_packet);
        }
    }
    public static void sendJPEGBuffer(int _toClient,byte[] buffer)
    {
        using(Packet _packet = new Packet((int)ServerPackets.JPEGBuffer))
        {
            _packet.Write(buffer.Length);
            _packet.Write(buffer);
            SendUDPData(_toClient, _packet);
        }
    }
}
