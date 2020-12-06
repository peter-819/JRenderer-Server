using System.Collections;
using System.Collections.Generic;
using System;
using System.Diagnostics;
using JRenderer_Server.src;

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
    public static void LoginDataReceived(int _fromClient, Packet _packet)
    {
        int clientIdCheck = _packet.ReadInt();
        if(_fromClient != clientIdCheck)
        {
            Console.WriteLine($"(ID: {_fromClient}) has assumed the wrong client ID ({clientIdCheck})");
            return;
        }
        string username = _packet.ReadString();
        string password = _packet.ReadString();
        if(username == "1234" && password == "123456")
        {
            ServerSend.sendLoginResult(_fromClient, true);
        }
        else
        {
            ServerSend.sendLoginResult(_fromClient, false);
        }
    }
    public static void InitInfoReceived(int _fromClient,Packet _packet)
    {
        int clientIdCheck = _packet.ReadInt();
        if (_fromClient != clientIdCheck)
        {
            Console.WriteLine($"(ID: {_fromClient}) has assumed the wrong client ID ({clientIdCheck})");
            return;
        }
        int width = _packet.ReadInt();
        int height = _packet.ReadInt();
        Renderer.Create(_fromClient, width, height);
        Renderer.instance.Run();
    }
    
    public static void MouseButtonEvent(int fromClient,Packet packet)
    {
        if(Renderer.instance.RendererHandle == IntPtr.Zero)
        {
            return;
        }
        int clientIdCheck = packet.ReadInt();
        if (fromClient != clientIdCheck)
        {
            Console.WriteLine($"(ID: {fromClient}) has assumed the wrong client ID ({clientIdCheck})");
            return;
        }
        byte button = packet.ReadByte();
        byte action = packet.ReadByte();
        //Console.WriteLine($"{button},{action}");

        Renderer.OnMouseButton(Renderer.instance.RendererHandle, button, action);
    }

    public static void MouseMoveEvent(int fromClient,Packet packet)
    {
        if (Renderer.instance.RendererHandle == IntPtr.Zero)
        {
            return;
        }
        int clientIdCheck = packet.ReadInt();
        if(fromClient != clientIdCheck)
        {
            Console.WriteLine($"(ID: {fromClient}) has assumed the wrong client ID ({clientIdCheck})");
            return;
        }
        int x = packet.ReadInt();
        int y = packet.ReadInt();
        //Console.WriteLine($"{x},{y}");

        Renderer.OnCursor(Renderer.instance.RendererHandle, x, y);
    }
    
    public static void CreateShape(int fromClient,Packet packet)
    {

        if (Renderer.instance.RendererHandle == IntPtr.Zero)
        {
            return;
        }
        int clientIdCheck = packet.ReadInt();
        if (fromClient != clientIdCheck)
        {
            Console.WriteLine($"(ID: {fromClient}) has assumed the wrong client ID ({clientIdCheck})");
            return;
        }
        int shape = packet.ReadInt();
        float scale = packet.ReadFloat();
        float x = packet.ReadFloat();
        float y = packet.ReadFloat();
        float z = packet.ReadFloat();

        Renderer.OnCreate(Renderer.instance.RendererHandle, shape,scale,x,y,z);
    }

    public static void CreateLight(int fromClient,Packet packet)
    {
        if (Renderer.instance.RendererHandle == IntPtr.Zero)
        {
            return;
        }
        int clientIdCheck = packet.ReadInt();
        if (fromClient != clientIdCheck)
        {
            Console.WriteLine($"(ID: {fromClient}) has assumed the wrong client ID ({clientIdCheck})");
            return;
        }

        float als=packet.ReadFloat();
        float dls=packet.ReadFloat();
        float slr=packet.ReadFloat();
        float slp=packet.ReadFloat();
        float x  =packet.ReadFloat();
        float y  =packet.ReadFloat();
        float z  =packet.ReadFloat();

        Renderer.OnCreateLight(Renderer.instance.RendererHandle, als, dls, slr, slp, x, y, z);
    }
}
