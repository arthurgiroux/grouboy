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
          darkest: "#0f380f",
          dark: "#306230",
          light: "#8bac0f",
          lightest: "#9bbc0f",
        },
        surface: {
          900: "#0a0a14",
          800: "#0f0f1a",
          700: "#161625",
          600: "#1e1e30",
          500: "#2a2a40",
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
          "0%, 100%": { boxShadow: "0 0 20px rgba(155, 188, 15, 0.15), 0 0 40px rgba(155, 188, 15, 0.05)" },
          "50%": { boxShadow: "0 0 30px rgba(155, 188, 15, 0.25), 0 0 60px rgba(155, 188, 15, 0.1)" },
        },
        borderGlow: {
          "0%, 100%": { borderColor: "rgba(155, 188, 15, 0.3)" },
          "50%": { borderColor: "rgba(155, 188, 15, 0.6)" },
        },
      },
      boxShadow: {
        "screen-inset": "inset 0 2px 12px rgba(0, 0, 0, 0.8), inset 0 0 4px rgba(0, 0, 0, 0.5)",
        "bezel": "0 4px 30px rgba(0, 0, 0, 0.5), 0 0 60px rgba(155, 188, 15, 0.07)",
      },
    },
  },
  plugins: [],
};
export default config;
