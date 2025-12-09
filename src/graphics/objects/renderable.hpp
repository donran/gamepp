namespace enginepp::graphics::objects {
struct Renderable {
    virtual float *Vertices() = 0;
    virtual unsigned int VerticesCount() = 0;
    virtual void AddStaticAttributes(struct VertexBufferAttributes *attrs) = 0;
    virtual void AddInstancedAttributes(struct VertexBufferAttributes *attrs) = 0;
    // virtual std::vector<float> InstanceData() = 0;
};
}; // namespace enginepp::graphics::objects
