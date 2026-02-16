import Emulator from "./emulator";

function GitHubIcon() {
  return (
    <svg viewBox="0 0 24 24" width="20" height="20" fill="currentColor" aria-hidden="true">
      <path d="M12 0C5.37 0 0 5.37 0 12c0 5.31 3.435 9.795 8.205 11.385.6.105.825-.255.825-.57 0-.285-.015-1.23-.015-2.235-3.015.555-3.795-.735-4.035-1.41-.135-.345-.72-1.41-1.23-1.695-.42-.225-1.02-.78-.015-.795.945-.015 1.62.87 1.845 1.23 1.08 1.815 2.805 1.305 3.495.99.105-.78.42-1.305.765-1.605-2.67-.3-5.46-1.335-5.46-5.925 0-1.305.465-2.385 1.23-3.225-.12-.3-.54-1.53.12-3.18 0 0 1.005-.315 3.3 1.23.96-.27 1.98-.405 3-.405s2.04.135 3 .405c2.295-1.56 3.3-1.23 3.3-1.23.66 1.65.24 2.88.12 3.18.765.84 1.23 1.905 1.23 3.225 0 4.605-2.805 5.625-5.475 5.925.435.375.81 1.095.81 2.22 0 1.605-.015 2.895-.015 3.3 0 .315.225.69.825.57A12.02 12.02 0 0024 12c0-6.63-5.37-12-12-12z" />
    </svg>
  );
}

function KeyBadge({ children }: { children: React.ReactNode }) {
  return <span className="key-badge">{children}</span>;
}

function ControlRow({ keys, label }: { keys: React.ReactNode; label: string }) {
  return (
    <div className="flex items-center gap-3">
      <div className="flex items-center gap-1">{keys}</div>
      <span className="text-slate-500 text-xs">=</span>
      <span className="text-slate-400 text-xs">{label}</span>
    </div>
  );
}

export default function Home() {
  return (
    <div className="min-h-screen flex flex-col">
      {/* Header */}
      <header className="w-full border-b border-white/5">
        <div className="max-w-4xl mx-auto px-6 py-4 flex items-center justify-between">
          <div className="flex items-center gap-3">
            <h1 className="text-xl font-bold tracking-wider text-gb-lightest">
              GROUBOY
            </h1>
            <span className="text-[10px] tracking-widest uppercase text-slate-600 border border-slate-700 rounded px-1.5 py-0.5">
              v0.1
            </span>
          </div>
          <a
            href="https://github.com/arthurgiroux/grouboy"
            target="_blank"
            rel="noopener noreferrer"
            className="text-slate-500 hover:text-gb-lightest transition-colors duration-200 flex items-center gap-2"
          >
            <GitHubIcon />
            <span className="text-xs hidden sm:inline">Source</span>
          </a>
        </div>
      </header>

      {/* Main content */}
      <main className="flex-1 flex flex-col items-center px-4 sm:px-6">
        {/* Tagline */}
        <div className="text-center mt-8 mb-8 animate-fade-in">
          <p className="text-slate-400 text-sm sm:text-base max-w-lg mx-auto leading-relaxed">
            A Game Boy emulator built in{" "}
            <span className="text-gb-light">C++</span>, running in your browser
            via <span className="text-gb-light">WebAssembly</span>
          </p>
        </div>

        {/* Emulator */}
        <div className="animate-fade-in-up w-full flex justify-center" style={{ animationDelay: "0.1s", animationFillMode: "backwards" }}>
          <Emulator />
        </div>

        {/* Controls guide */}
        <div className="mt-10 mb-8 animate-fade-in w-full max-w-2xl" style={{ animationDelay: "0.3s", animationFillMode: "backwards" }}>
          <div className="rounded-2xl bg-surface-700/40 border border-white/5 p-6">
            <h2 className="text-xs font-bold tracking-widest uppercase text-slate-500 mb-4">
              Controls
            </h2>
            <div className="grid grid-cols-2 sm:grid-cols-3 gap-x-8 gap-y-3">
              <ControlRow
                keys={
                  <>
                    <KeyBadge>&#8593;</KeyBadge>
                    <KeyBadge>&#8595;</KeyBadge>
                    <KeyBadge>&#8592;</KeyBadge>
                    <KeyBadge>&#8594;</KeyBadge>
                  </>
                }
                label="D-Pad"
              />
              <ControlRow keys={<KeyBadge>A</KeyBadge>} label="A Button" />
              <ControlRow keys={<KeyBadge>B</KeyBadge>} label="B Button" />
              <ControlRow keys={<KeyBadge>Enter</KeyBadge>} label="Start" />
              <ControlRow keys={<KeyBadge>Space</KeyBadge>} label="Select" />
              <ControlRow keys={<KeyBadge>D</KeyBadge>} label="Debug" />
            </div>
          </div>
        </div>
      </main>

      {/* Footer */}
      <footer className="w-full border-t border-white/5 py-6">
        <div className="max-w-4xl mx-auto px-6 flex flex-col sm:flex-row items-center justify-between gap-2">
          <p className="text-slate-600 text-xs">
            Built with C++ &middot; Emscripten &middot; SDL2 &middot; Next.js
          </p>
          <p className="text-slate-700 text-xs">
            &copy; {new Date().getFullYear()} Arthur Giroux
          </p>
        </div>
      </footer>
    </div>
  );
}
