from abc import ABC, abstractmethod

class AbstractBench(ABC):
    @abstractmethod
    def to_html(self):
        raise NotImplementedError("Abstract method call")
    
    @abstractmethod
    def gen_images(self):
        raise NotImplementedError("Abstract method call")
    
    @abstractmethod
    def get_index(self):
        raise NotImplementedError("Abstract method call")
