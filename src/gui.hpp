class Gui {
	public:
		Gui();
		~Gui();
		void run();
		bool isValid();
		bool isRunning();
	
	private:
		bool valid = true;
		bool running = false;
};

