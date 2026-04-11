import type { Config } from "tailwindcss";

const config: Config = {
  content: [
    "./pages/**/*.{js,ts,jsx,tsx,mdx}",
    "./components/**/*.{js,ts,jsx,tsx,mdx}",
    "./app/**/*.{js,ts,jsx,tsx,mdx}",
  ],
  theme: {
    extend: {
      colors: {
        gb: {
          darkest: "#3b0764",
          dark: "#7c3aed",
          light: "#a855f7",
          lightest: "#c084fc",
        },
        surface: {
          900: "#09070f",
          800: "#0e0b1a",
          700: "#160f2a",
          600: "#1e1438",
          500: "#2a1d4a",
        },
      },
      fontFamily: {
        mono: ['"Space Mono"', "monospace"],
      },
      animation: {
        "fade-in": "fadeIn 0.5s ease-out forwards",
        "fade-in-up": "fadeInUp 0.6s ease-out forwards",
        "glow-pulse": "glowPulse 3s ease-in-out infinite",
        "border-glow": "borderGlow 3s ease-in-out infinite",
      },
      keyframes: {
        fadeIn: {
          "0%": { opacity: "0" },
          "100%": { opacity: "1" },
        },
        fadeInUp: {
          "0%": { opacity: "0", transform: "translateY(16px)" },
          "100%": { opacity: "1", transform: "translateY(0)" },
        },
        glowPulse: {
          "0%, 100%": { boxShadow: "0 0 20px rgba(168, 85, 247, 0.15), 0 0 40px rgba(168, 85, 247, 0.05)" },
          "50%": { boxShadow: "0 0 30px rgba(168, 85, 247, 0.25), 0 0 60px rgba(168, 85, 247, 0.1)" },
        },
        borderGlow: {
          "0%, 100%": { borderColor: "rgba(168, 85, 247, 0.3)" },
          "50%": { borderColor: "rgba(168, 85, 247, 0.6)" },
        },
      },
      boxShadow: {
        "screen-inset": "inset 0 2px 12px rgba(0, 0, 0, 0.8), inset 0 0 4px rgba(0, 0, 0, 0.5)",
        "bezel": "0 4px 30px rgba(0, 0, 0, 0.6), 0 0 60px rgba(168, 85, 247, 0.1)",
      },
    },
  },
  plugins: [],
};
export default config;
