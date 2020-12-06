using System.Collections;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System;
/// <summary>
/// 服务端类，控制服务器启停，为连接的用户分配Client （只需增减Handle处理收到信号）
/// </summary>
public class Server 
{
    public static int MaxPlayers { get; private set; }
    public static int Port { get; private set; }
    public static Dictionary<int, Client> clients = new Dictionary<int, Client>();
    public delegate void PacketHandler(int _fromClient, Packet _packet);
    public static Dictionary<int, PacketHandler> packetHandlers;

    private static TcpListener m_TcpListener;
    private static UdpClient m_UdpListener;
    public static void Start(int maxplayers, int port)
    {
        MaxPlayers = maxplayers;
        Port = port;
        Console.WriteLine("Starting AOH Server...");
        InitializeServerData();

        m_TcpListener = new TcpListener(IPAddress.Any, port);
        m_TcpListener.Start();
        m_TcpListener.BeginAcceptTcpClient(TCPConnectCallback, null);

        m_UdpListener = new UdpClient(Port);
        m_UdpListener.BeginReceive(UDPReceiveCallback, null);

        Console.WriteLine($"AOH Server Started on {port}.");
    }

    private static void TCPConnectCallback(IAsyncResult result)
    {
        TcpClient client = m_TcpListener.EndAcceptTcpClient(result);
        m_TcpListener.BeginAcceptTcpClient(new AsyncCallback(TCPConnectCallback), null);
        Console.WriteLine($"Incoming connection from {client.Client.RemoteEndPoint} ...");
        for (int i = 1; i <= MaxPlayers; i++)
        {
            if (clients[i].tcp.socket == null)
            {
                clients[i].tcp.Connect(client);
                return;
            }
        }
        Console.WriteLine($"{client.Client.RemoteEndPoint} failed to connect : Server Full!");
    }
    private static void UDPReceiveCallback(IAsyncResult _result)
    {
        try
        {
            IPEndPoint _clientEndPoint = new IPEndPoint(IPAddress.Any, 0);
            byte[] _data = m_UdpListener.EndReceive(_result, ref _clientEndPoint);
            m_UdpListener.BeginReceive(UDPReceiveCallback, null);

            if (_data.Length < 4)
            {
                return;
            }

            using (Packet _packet = new Packet(_data))
            {
                int _clientId = _packet.ReadInt();

                if (_clientId == 0)
                {
                    return;
                }

                if (clients[_clientId].udp.endPoint == null)
                {
                    clients[_clientId].udp.Connect(_clientEndPoint);
                    return;
                }

                if (clients[_clientId].udp.endPoint.ToString() == _clientEndPoint.ToString())
                {
                    clients[_clientId].udp.HandleData(_packet);
                }
            }
        }
        catch (Exception _ex)
        {
            Console.WriteLine($"Error receiving UDP data: {_ex}");
        }
    }
    public static void SendUDPData(IPEndPoint _clientEndPoint, Packet _packet)
    {
        try
        {
            if (_clientEndPoint != null)
            {
                m_UdpListener.BeginSend(_packet.ToArray(), _packet.Length(), _clientEndPoint, null, null);
            }
        }
        catch (Exception _ex)
        {
            Console.WriteLine($"Error sending data to {_clientEndPoint} via UDP: {_ex}");
        }
    }
    /// <summary>
    /// 注意增减Handle函数
    /// </summary>
    private static void InitializeServerData()
    {
        for (int i = 1; i <= MaxPlayers; i++)
        {
            clients.Add(i, new Client(i));
        }
        packetHandlers = new Dictionary<int, PacketHandler>()
            {
                {(int)ClientPackets.welcomeReceived,ServerHandle.WelcomeReceived },
                {(int)ClientPackets.sendString,ServerHandle.StringReceived },
                {(int)ClientPackets.sendDataPackets,ServerHandle.DataReceived },
                {(int)ClientPackets.sendLoginData,ServerHandle.LoginDataReceived },
                {(int)ClientPackets.sendInitInfo,ServerHandle.InitInfoReceived },
                {(int)ClientPackets.sendMouseButtonEvent,ServerHandle.MouseButtonEvent },
                {(int)ClientPackets.sendMouseMoveEvent,ServerHandle.MouseMoveEvent },
                {(int)ClientPackets.sendCreateSignal,ServerHandle.CreateShape },
                {(int)ClientPackets.sendLightData,ServerHandle.CreateLight }
            };
        Console.WriteLine("Initalized packets.");
    }

    public static void Stop()
    {
        m_TcpListener.Stop();
        m_UdpListener.Close();
    }
}
