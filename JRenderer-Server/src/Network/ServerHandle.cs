using System.Collections;
using System.Collections.Generic;
using System;

/// <summary>
/// 处理服务端收到信息的函数
/// </summary>
public class ServerHandle 
{
    /// <summary>
    /// 得到客户端发送的username
    /// </summary>
    /// <param name="_fromClient">来自的客户端编号</param>
    /// <param name="_packet">Packet包含：ID，Username</param>
    public static void WelcomeReceived(int _fromClient, Packet _packet)
    {
        int _clientIdCheck = _packet.ReadInt();
        string _username = _packet.ReadString();

        Console.WriteLine($"{Server.clients[_fromClient].tcp.socket.Client.RemoteEndPoint} connected successfully and is now player {_fromClient}");
        if (_fromClient != _clientIdCheck)
        {
            Console.WriteLine($"Player\"{_username}\" (ID: {_fromClient}) has assumed the wrong client ID ({_clientIdCheck})");
        }
    }
    public static void StringReceived(int _fromClient,Packet _packet)
    {
        int _clientIdCheck = _packet.ReadInt();
        string str = _packet.ReadString();

        Console.WriteLine($"{str} FROM: {_fromClient}");
        if (_fromClient != _clientIdCheck)
        {
            Console.WriteLine($"(ID: {_fromClient}) has assumed the wrong client ID ({_clientIdCheck})");
        }
    }
    public static void DataReceived(int _fromClient,Packet _packet)
    {
        int _clientIdCheck = _packet.ReadInt();
        int length = _packet.ReadInt();
        byte[] data = _packet.ReadBytes(length);

        foreach(var a in data) { Console.Write(a); }
        if (_fromClient != _clientIdCheck)
        {
            Console.WriteLine($"(ID: {_fromClient}) has assumed the wrong client ID ({_clientIdCheck})");
        }
    }
}
