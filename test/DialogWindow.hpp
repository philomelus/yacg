#ifndef	__DIALOGWINDOW_HPP__
#define	__DIALOGWINDOW_HPP__

class _DialogWindow1 : public yacg::_Dialog
{
public:
	_DialogWindow1(int i = 0)
			:
			_Dialog(100, 100, 500, 500),
			_exit(0)
	{
	}

protected:
	void init()
	{
		_exit = new yacg::Button(*this, right_controls() - 220, bottom_controls() - 40,
				200, 20, "Exit");
		insert(*_exit);
		_exit->callback(exit, this);
	}

private:
	static void exit(yacg::Button& c, void* data, BITMAP* b)
	{
		reinterpret_cast<_DialogWindow1*>(data)->close();
	}
	
private:
	yacg::Button* _exit;
};
typedef yacg::Dialog<_DialogWindow1> DialogWindow1;

class _DialogWindow2 : public yacg::_Dialog
{
public:
	_DialogWindow2(int p1 = 0, int p2 = 0)
			:
			_Dialog(500, 500, "Test Dialog 2"),
			_exit(0)
	{
	}

protected:
	void init()
	{
		_exit = new yacg::Button(*this, right_controls() - 220, bottom_controls() - 40,
				200, 20, "Exit");
		insert(*_exit);
		_exit->callback(exit, this);
	}

private:
	static void exit(yacg::Button& c, void* data, BITMAP* b)
	{
		reinterpret_cast<_DialogWindow2*>(data)->close();
	}
	
private:
	yacg::Button* _exit;
};
typedef yacg::Dialog<_DialogWindow2> DialogWindow2;

#endif
