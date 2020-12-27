#pragma once

struct Session
{
	int m_ID;
	string m_Name;
	time_t m_Time;

	queue<Message> m_Messages;
	CCriticalSection m_CS;

	Session(int ID, string Name)
		:m_ID(ID), m_Name(Name), m_Time(time(0))
	{
	}

	void Add(Message& m)
	{
		CSingleLock sl(&m_CS, TRUE);
		m_Messages.push(m);
	}

	void Send(CSocket& s)
	{
		CSingleLock sl(&m_CS, TRUE);
		if (m_Messages.empty())
		{
			Message::Send(s, m_ID, M_BROKER, M_NODATA);
		}
		else
		{
			m_Messages.front().Send(s);
			m_Messages.pop();
		}
	}

};