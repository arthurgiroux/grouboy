import type { Metadata } from "next";
import { Space_Mono } from "next/font/google";
import "./globals.css";

const spaceMono = Space_Mono({
  weight: ["400", "700"],
  subsets: ["latin"],
  display: "swap",
});

export const metadata: Metadata = {
  title: "Grouboy - Game Boy Emulator",
  description: "A Game Boy emulator built in C++, running in your browser via WebAssembly",
};

export default function RootLayout({
  children,
}: Readonly<{
  children: React.ReactNode;
}>) {
  return (
    <html lang="en" className="dark">
      <body className={`${spaceMono.className} bg-surface-900 min-h-screen antialiased`}>
        {children}
      </body>
    </html>
  );
}
