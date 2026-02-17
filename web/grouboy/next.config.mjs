/** @type {import('next').NextConfig} */
const nextConfig = {
    basePath: "/grouboy",
    output: "export",
    webpack: (config) => {
        config.experiments = {
            ...config.experiments,
            asyncWebAssembly: true,
        };
        config.module.rules.push({
            test: /\.wasm$/,
            type: "asset/resource",
        });
        return config;
    },
};

export default nextConfig;
